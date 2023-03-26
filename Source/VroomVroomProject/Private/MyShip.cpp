// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShip.h"
#include "Math/NumericLimits.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AMyShip::AMyShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	originalTurningBoost_ = floatingPawnMovement_->TurningBoost;
	originalDeceleration_ = floatingPawnMovement_->Deceleration;
}

// Called when the game starts or when spawned
void AMyShip::BeginPlay()
{
	Super::BeginPlay();
	lastRotation_ = GetActorUpVector();
}

void AMyShip::MoveShip(float DeltaTime)
{
	FVector cameraDirectionForward = UKismetMathLibrary::GetForwardVector(springArm_->GetComponentRotation());
	FVector cameraDirectionRight = UKismetMathLibrary::GetRightVector(springArm_->GetComponentRotation());
	FVector inputDirection = cameraDirectionForward * axisX_ + cameraDirectionRight * axisY_;
	inputDirection.Normalize();

	if (accelInput_ != 0.0f && inputDirection.IsNearlyZero()) {
		inputDirection = cameraDirectionForward * 1.0f + cameraDirectionRight * axisY_;
	}

	FRotator playerRot = GetActorRotation();
	playerRot = FMath::Lerp(GetActorRotation(), currentRotation, speedOfRotation_ * DeltaTime * 8.0f);


	if (axisY_ != 0.0f)
	{
		speedMultiplier_ = FMath::Lerp(0.0f, 10.0f, speedOfRotation_ * DeltaTime * 2.0f);
	}
	else
	{
		speedMultiplier_ = FMath::Lerp(speedMultiplier_, 0.0f, speedOfRotation_ * DeltaTime * 2.0f);
	}

	if (!isFalling_)
	{
		SetActorRotation(playerRot);
		AddActorLocalRotation(FRotator(0, axisY_ * speedMultiplier_, 0));
	}

}

void AMyShip::RotateShip(float DeltaTime)
{
	FHitResult hit;
	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(this);

	FVector center;
	FHitResult hit3;

	if (GetWorld()->LineTraceSingleByChannel(hit3, GetActorLocation() + 10.0f, GetActorLocation() - lastRotation_ * (desiredHeight_ * 20.0f), ECC_Visibility, traceParams)) {
		center = hit3.ImpactPoint + hit3.ImpactNormal * (desiredHeight_);
		shipMesh_->SetEnableGravity(false);
		isFalling_ = false;
		justDetached_ = false;
	}
	else
	{		
		isFalling_ = true;
		if (!justDetached_)
		{
			justDetached_ = true;
			lastShipPosition_ = GetActorLocation();
		}
	}

	if (debug)DrawDebugSphere(GetWorld(), center, 100.0f, 32, FColor::Yellow);

	FVector resultingPos;
	if (center.Z >= GetActorLocation().Z) {
		resultingPos = FMath::Lerp(GetActorLocation(), FVector(GetActorLocation().X, GetActorLocation().Y, center.Z), (DeltaTime * speedOfGoingToHeight_));
	}
	else {
		resultingPos = FMath::Lerp(GetActorLocation(), FVector(GetActorLocation().X, GetActorLocation().Y, center.Z), (DeltaTime * speedOfGoingToHeight_) / (FVector::Dist(hit.ImpactPoint, center) / gravitySpeed_));
	}
	SetActorLocation(resultingPos);

	FRotator rotation = UKismetMathLibrary::MakeRotFromYZ(GetActorRightVector(), hit3.ImpactNormal);

	currentRotation = rotation;

	lastRotation_ = hit3.ImpactNormal;
}

void AMyShip::StartDrift()
{
	isDrifiting_ = true;

	green_ = GetActorForwardVector();
	blue_ = green_.RotateAngleAxis(FMath::CeilToInt(axisY_) * 90.0f, GetActorUpVector());
	yellow_ = green_;
	angleAxis_ = 0.0f;
}

void AMyShip::StopDrift()
{
	isDrifiting_ = false;
}

void AMyShip::CameraLookAtPlayer()
{
	FRotator lookRot = UKismetMathLibrary::FindLookAtRotation(lastShipPosition_, GetActorLocation());
	cameraComponent_->SetWorldRotation(lookRot);
}

void AMyShip::ForwardAxis(float input)
{
	axisX_ = input;
}

void AMyShip::SideAxis(float input)
{
	axisY_ = input;
}

void AMyShip::Accelerate(float input)
{
	accelInput_ = input;

	FVector A = GetActorLocation() + GetActorForwardVector(); //green
	FVector M = GetActorLocation();							  //centre
	FVector B = GetActorLocation() + blue_;					  //blue	
	

	FVector center = ((A + B) / 2);

	red_ = center - M;

	A.Normalize();
	red_.Normalize();
	blue_.Normalize();

	if (angleAxis_ > 360.0f)
	{
		angleAxis_ = 1.0f;
	}


	if (isDrifiting_)
	{
		if (axisY_ != 0.0f)
		{
			angleAxis_ = axisY_ * 50.0f * deltaTime_;
			//angleAxis_ = FMath::Clamp(angleAxis_, green_.Rotation().Yaw * axisY_, red_.Rotation().Yaw * axisY_);
			yellow_ = yellow_.RotateAngleAxis(angleAxis_, GetActorUpVector());

			//float pitch = FMath::Clamp(GetActorRotation().Yaw, green_.Rotation().Yaw * axisY_, blue_.Rotation().Yaw * axisY_);
			//SetActorRotation(FRotator(0.0f, pitch, 0.0f));

			//blue_ = green_.RotateAngleAxis(FMath::CeilToInt(axisY_) * 90.0f, GetActorUpVector());
			AddMovementInput(green_, input);
		}

		floatingPawnMovement_->TurningBoost = originalTurningBoost_ - 100.0f;
		floatingPawnMovement_->Deceleration = originalDeceleration_ - 100.0f;
		springArm_->SocketOffset.Y *= axisY_ + 1.0f;
		springArm_->SocketOffset.Y = FMath::Clamp(springArm_->SocketOffset.Y, 0.0f , 2.0f);
	}
	else
	{
		floatingPawnMovement_->TurningBoost = FMath::Lerp(floatingPawnMovement_->TurningBoost, originalTurningBoost_, GetWorld()->DeltaTimeSeconds * 2.0f);
		floatingPawnMovement_->Deceleration = FMath::Lerp(floatingPawnMovement_->Deceleration, originalDeceleration_, GetWorld()->DeltaTimeSeconds * 5.0f);
		springArm_->SocketOffset.Y = FMath::Lerp(springArm_->SocketOffset.Y, 0.0f, GetWorld()->DeltaTimeSeconds * 1.5f);

		AddMovementInput(GetActorForwardVector(), input);
	}

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + yellow_ * 500.0f, FColor::Yellow, false, -1.0f, 300.0f);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + green_ * 1000.0f, FColor::Green, false, -1.0f, 100.0f);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + red_ * 1000.0f, FColor::Red, false, -1.0f, 100.0f);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + blue_ * 1000.0f, FColor::Blue, false, -1.0f, 100.0f);
}

void AMyShip::MoveCameraX(float input)
{
	//springArm_->AddRelativeRotation(FRotator(0, input, 0));
}

// Called every frame
void AMyShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	deltaTime_ = DeltaTime;

	MoveShip(DeltaTime);
	RotateShip(DeltaTime);

	if (!isFalling_)
	{
		shipMesh_->SetSimulatePhysics(false);
		shipMesh_->SetEnableGravity(false);
	}
	else
	{
		cameraComponent_->SetWorldLocation(lastShipPosition_);
		CameraLookAtPlayer();
		shipMesh_->SetSimulatePhysics(true);
		shipMesh_->SetEnableGravity(true);
		springArm_->DetachFromParent();
		floatingPawnMovement_->TurningBoost = 0.0f;
		floatingPawnMovement_->Deceleration = 1000.0f;
	}

}

// Called to bind functionality to input
void AMyShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (PlayerInputComponent) {
		PlayerInputComponent->BindAxis("MoveForward", this, &AMyShip::ForwardAxis);
		PlayerInputComponent->BindAxis("Accel", this, &AMyShip::Accelerate);
		PlayerInputComponent->BindAxis("MoveLateral", this, &AMyShip::SideAxis);
		PlayerInputComponent->BindAxis("LookRight", this, &AMyShip::MoveCameraX);
		PlayerInputComponent->BindAction("Drift", IE_Pressed, this, &AMyShip::StartDrift);
		PlayerInputComponent->BindAction("Drift", IE_Released, this, &AMyShip::StopDrift);
	}
}


void AMyShip::PickItem_Implementation(TSubclassOf<AItem> pItem)
{
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	item = Cast<AItem>(GetWorld()->SpawnActor(pItem.Get(), &Location, &Rotation, SpawnParams));
	item->SetPlayerOwner(this);
}

void AMyShip::FreezeInput_Implementation(float duration)
{
	if (invincible)
		return;

	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	GetWorldTimerManager().SetTimer(inputDisabled, this, &AMyShip::RestoreInput_Implementation, duration);
}

void AMyShip::RestoreInput_Implementation()
{
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void AMyShip::UseItem_Implementation()
{
	if (item)
	{
		item->Execute();
		item = nullptr;
	}
}