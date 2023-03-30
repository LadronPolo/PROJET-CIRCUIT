
#include "car.h"
#include "Math/NumericLimits.h"
#include "Kismet/KismetMathLibrary.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

Acar::Acar()
{
	shipMesh_ = CreateDefaultSubobject<UStaticMeshComponent>("ShipBody");
	shipMesh_->SetupAttachment(RootComponent);

	hoverPoint1_ = CreateDefaultSubobject<UStaticMeshComponent>("ShipBody2");
	hoverPoint1_->SetupAttachment(shipMesh_);
	hoverPoint2_ = CreateDefaultSubobject<UStaticMeshComponent>("ShipBody3");
	hoverPoint2_->SetupAttachment(shipMesh_);
	hoverPoint3_ = CreateDefaultSubobject<UStaticMeshComponent>("ShipBody4");
	hoverPoint3_->SetupAttachment(shipMesh_);

	floatingPawnMovement_ = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingMovement");

	springArm_ = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	springArm_->SetupAttachment(shipMesh_);

	cameraComponent_ = CreateDefaultSubobject<UCameraComponent>("Camera");
	cameraComponent_->SetupAttachment(springArm_);

	speedOfRotation_ = 10.0f;
	levitateHeight_ = 10.0f;
	timePast_ = 0.0f;
	levitateSpeed_ = 5.0f;
	gravitySpeed_ = 150.0f;
	distanceToPlayer_ = 10.0f;
	speed_ = 0.0f;

	originalTurningBoost_ = floatingPawnMovement_->TurningBoost;
	originalDeceleration_ = floatingPawnMovement_->Deceleration;


	EnergyChargeRate = 0;
}

void Acar::BeginPlay()
{
	Super::BeginPlay();
	lastRotation_ = GetActorUpVector();
}

void Acar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	deltaTime_ = DeltaTime;

	if (accelInput_ == 0.0f)
	{
		decelerationTimer_ += deltaTime_;
		decelerationTimer_ = FMath::Clamp(decelerationTimer_, 0.0f, 1.0f);

		timer_ = 0.0f;
		currentForwardSpeed_ = FMath::Lerp(currentForwardSpeed_, 0.0f, decelerationTimer_ * 1000.0f * DeltaTime);

		shipMesh_->SetAllPhysicsLinearVelocity(FMath::Lerp(shipMesh_->GetComponentVelocity(), FVector::Zero(), deltaTime_));

		//// Remove Forward Force
		//shipMesh_->AddForce(GetActorForwardVector() * -currentForwardSpeed_);

		////Remove Turning Force
		//shipMesh_->AddForce(GetActorRightVector() * -currentLateralSpeed_);
	}


	if (boost_)
	{
		//if (canGetDriftBoost_)
		//{
		boostTimer_ += deltaTime_;

		if (boostTimer_ >= 0.2f)
		{
			boost_ = false;
			canGetDriftBoost_ = false;
			boostTimer_ = 0.0f;
		}

		boostAmount_ = 2.0f;
		//}

	}
	else
	{
		boostAmount_ = 1.0f;
	}



	MoveShip(DeltaTime);
	RotateShip(DeltaTime);


	EnergyRemaining = EnergyRemaining < EnergyMax ? (EnergyRemaining + DeltaTime * EnergyChargeRate) : EnergyMax;
	BoostAvaillable = EnergyRemaining >= EnergyNeededToBoost;
}

void Acar::MoveShip(float DeltaTime)
{
	FVector cameraDirectionForward = UKismetMathLibrary::GetForwardVector(springArm_->GetComponentRotation());
	FVector cameraDirectionRight = UKismetMathLibrary::GetRightVector(springArm_->GetComponentRotation());
	FVector inputDirection;

	if (accelInput_ != 0.0f && inputDirection.IsNearlyZero()) {
		inputDirection = cameraDirectionForward * 1.0f + cameraDirectionRight * axisY_;
	}

	FRotator playerRot = GetActorRotation();
	playerRot = FMath::Lerp(GetActorRotation(), currentRotation, speedOfRotation_ * DeltaTime * 2.0f);

	if (axisY_ != 0.0f)
	{
		speedMultiplier_ = FMath::Lerp(0.0f, 10.0f, speedOfRotation_ * DeltaTime * 2.0f);
	}
	else
	{
		speedMultiplier_ = FMath::Lerp(speedMultiplier_, 0.0f, speedOfRotation_ * DeltaTime * 2.0f);
	}

	float lerp = FMath::Lerp(shipMesh_->GetRelativeRotation().Roll, axisY_ * 50.0f, DeltaTime * 2.0f);
	FRotator newRot = shipMesh_->GetRelativeRotation();
	newRot.Roll = lerp;
	shipMesh_->SetRelativeRotation(newRot);
	AddActorLocalRotation(FRotator(0, axisY_ * speedMultiplier_, 0));

}

void Acar::RotateShip(float DeltaTime)
{
	FHitResult hit;
	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(this);

	FVector center;
	FHitResult hit3;

	if (GetWorld()->LineTraceSingleByChannel(hit3, GetActorLocation(), GetActorLocation() - lastRotation_ * (desiredHeight_ * 1000.0f), ECC_Visibility, traceParams)) {
		center = hit3.ImpactPoint + hit3.ImpactNormal * (desiredHeight_);
	}

	if (debug)DrawDebugSphere(GetWorld(), center, 100.0f, 32, FColor::Yellow);

	if (!isFalling_)
	{
		FVector resultingPos;
		resultingPos = FMath::Lerp(GetActorLocation(), center, (DeltaTime * speedOfGoingToHeight_));
		SetActorLocation(resultingPos);
	}


	FVector UpVector = GetActorUpVector();
	FVector NormalVector = hit3.ImpactNormal;

	FVector RotationAxis = FVector::CrossProduct(UpVector, NormalVector);
	RotationAxis.Normalize();

	float DotProduct = FVector::DotProduct(UpVector, NormalVector);
	float RotationAngle = acosf(DotProduct);

	FQuat Quat = FQuat(RotationAxis, RotationAngle);
	FQuat RootQuat = GetActorQuat();

	FQuat NewQuat = Quat * RootQuat;

	SetActorRotation(FQuat::Slerp(GetActorRotation().Quaternion(), NewQuat, 10.0f * DeltaTime));

	lastRotation_ = hit3.ImpactNormal;
}

void Acar::StartDrift()
{
	isDrifiting_ = true;
	red_ = GetActorForwardVector();
	green_ = GetActorForwardVector();
	blue_ = green_.RotateAngleAxis(FMath::CeilToInt(axisY_) * 90.0f, GetActorUpVector());
	yellow_ = green_;
	angleAxis_ = 0.0f;
	boost_ = false;

}

void Acar::StopDrift()
{
	isDrifiting_ = false;
	boost_ = true;
}

void Acar::CameraLookAtPlayer()
{
	FRotator lookRot = UKismetMathLibrary::FindLookAtRotation(lastShipPosition_, GetActorLocation());
	cameraComponent_->SetWorldRotation(lookRot);
}

void Acar::SideAxis(float input)
{
	axisY_ = input;
}

void Acar::Accelerate(float input)
{
	accelInput_ = input;

	currentForwardSpeed_ = FMath::Lerp(currentForwardSpeed_, ((500000.0f) / 2.0f) * input, deltaTime_ * 2.0f);

	currentLateralSpeed_ = FMath::Lerp(currentLateralSpeed_, ((500000.0f) / 2.0f) * axisY_, deltaTime_ * 2.0f);

	if (input == 0.0f)
	{
		decelerationTimer_ = 0.0f;
		return;
	}

	green_.Normalize();


	if (angleAxis_ > 360.0f)
	{
		angleAxis_ = 1.0f;
	}

	angleAxis_ += deltaTime_ * boosterLoopSpeed_;
	angleAxis_ = FMath::Clamp(angleAxis_, 0.0f, 500.0f);
	yellow_ = yellow_.RotateAngleAxis(angleAxis_, GetActorUpVector());
	float BoosterLoopDotProduct = FVector::DotProduct(yellow_, GetActorForwardVector());


	if (BoosterLoopDotProduct >= 0.90f)
	{
		boosterLoopCount_ += 1.0f;
		canGetDriftBoost_ = true;
	}
	else
	{
		canGetDriftBoost_ = false;
	}

	if (isDrifiting_)
	{

		floatingPawnMovement_->TurningBoost = originalTurningBoost_ - 500.0f;
		floatingPawnMovement_->Deceleration = originalDeceleration_ - 1000.0f;

		FVector MiddleVector = FMath::Lerp(green_, GetActorForwardVector(), 0.5f);
		FVector DriftLocation = GetActorLocation() + ((MiddleVector)*deltaTime_ * speed_);

		float DotProduct = FVector::DotProduct(MiddleVector, GetActorForwardVector());
		float RotationAngle = acosf(DotProduct);
		float AngleInDegrees = FMath::RadiansToDegrees(RotationAngle);

		if (AngleInDegrees > 50.0f)
		{
			FVector NewMiddleVector = FMath::Lerp(green_, GetActorForwardVector(), 0.5f);
			green_ = NewMiddleVector;
		}

		//Drift Force
		shipMesh_->AddForce(green_ * currentLateralSpeed_);
		shipMesh_->AddForce(GetActorForwardVector() * currentForwardSpeed_);

	}
	else
	{
		angleAxis_ = 0.0f;
		boosterLoopCount_ = 0.0f;

		floatingPawnMovement_->TurningBoost = FMath::Lerp(floatingPawnMovement_->TurningBoost, originalTurningBoost_, GetWorld()->DeltaTimeSeconds * 10.0f);
		floatingPawnMovement_->Deceleration = FMath::Lerp(floatingPawnMovement_->Deceleration, originalDeceleration_, GetWorld()->DeltaTimeSeconds * 10.0f);

		//Forward Force
		shipMesh_->AddForce(GetActorForwardVector() * currentForwardSpeed_ * boostAmount_);

		//Turning Force
		shipMesh_->AddForce(GetActorRightVector() * currentLateralSpeed_ * boostAmount_);

	}

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + ((green_ + GetActorForwardVector()) / 2.0f) * 500.0f, FColor::Yellow, false, -1.0f, 500.0f);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 1000.0f, FColor::Blue, false, -1.0f, 500.0f);

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + yellow_ * 1000.0f, FColor::Green, false, -1.0f, 1000.0f);

}

void Acar::PickItem_Implementation(TSubclassOf<AItem> pItem)
{
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	item = Cast<AItem>(GetWorld()->SpawnActor(pItem.Get(), &Location, &Rotation, SpawnParams));
	item->SetPlayerOwner(this);
}

void Acar::FreezeInput_Implementation(float duration)
{
	if (invincible)
		return;

	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	GetWorldTimerManager().SetTimer(inputDisabled, this, &Acar::ResetInputTrigger, duration);
}

void Acar::ResetInputTrigger()
{
	IAbilityPawn::Execute_RestoreInput(this);
}

void Acar::RestoreInput_Implementation()
{
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void Acar::SetInvincible_Implementation(float duration)
{
	invincible = true;
	GetWorldTimerManager().SetTimer(invincibleTimer, this, &Acar::DisableInvincibleTrigger, duration);
}

void Acar::DisableInvincibleTrigger()
{
	IAbilityPawn::Execute_DisableInvincible(this);
}

void Acar::DisableInvincible_Implementation()
{
	invincible = false;
}

void Acar::UseItem_Implementation()
{
	if (item)
	{
		item->Execute();
		item = nullptr;
	}
}

void Acar::Miniaturize_Implementation(float duration)
{
	SetActorScale3D(FVector(0.25f, 0.25f, 0.25f));
	GetWorldTimerManager().SetTimer(miniatureTimer, this, &Acar::ResetMiniaturizeTrigger, duration);
}

void Acar::ResetMiniaturizeTrigger()
{
	IAbilityPawn::Execute_ResetMiniaturize(this);
}

void Acar::ResetMiniaturize_Implementation()
{
	SetActorScale3D(FVector(1, 1, 1));
}

void Acar::AddEnergy_Implementation(float energy)
{
	EnergyRemaining = EnergyRemaining < EnergyMax ? (EnergyRemaining + energy) : EnergyMax;
}

float Acar::getEnergyRemaining()
{
	return EnergyRemaining;
}

bool Acar::canBoost()
{
	return BoostAvaillable;
}

bool Acar::IsBoost()
{
	return IsBoosting;
}

void Acar::SetIsBoost(bool pState)
{
	IsBoosting = pState;
}

void Acar::SetEnergyChargeRate(float pValue)
{
	EnergyChargeRate = pValue;
}

float Acar::getEnergyChargeRate()
{
	return EnergyChargeRate;
}

bool Acar::IsInvincible()
{
	return invincible;
}

void Acar::spendBoost()
{
	EnergyRemaining -= BoostCost;
	if (EnergyRemaining < 0)
		EnergyRemaining = 0;
}

float Acar::getBoostCost()
{
	return BoostCost;
}

void Acar::SetEnergyRemaining(float pValue)
{
	EnergyRemaining = pValue;
}