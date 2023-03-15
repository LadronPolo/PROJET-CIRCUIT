// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShip.h"
#include "Math/NumericLimits.h"
#include "Kismet/KismetMathLibrary.h"

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

	speedOfRotation_ = 10.0f;
	levitateHeight_ = 10.0f;
	timePast_ = 0.0f;
	levitateSpeed_ = 5.0f;
	gravitySpeed_ = 150.0f;
}

// Called when the game starts or when spawned
void AMyShip::BeginPlay()
{
	Super::BeginPlay();
	
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

	if (!inputDirection.IsNearlyZero() || accelInput_ != 0.0f) {
		FVector currentDirection = GetActorRotation().Vector();
		currentDirection.Normalize();

		float dot = FVector::DotProduct(GetActorRightVector(), inputDirection);

		FRotator playerRot = FMath::Lerp(GetActorRotation(), inputDirection.Rotation(), speedOfRotation_ * DeltaTime);
		playerRot.Pitch = FMath::Lerp(GetActorRotation().Pitch, currentRotation.Pitch, speedOfRotation_ * DeltaTime * 1.25f);
		playerRot.Roll = FMath::Lerp(GetActorRotation().Roll, currentRotation.Roll + dot * 50.0f, speedOfRotation_ * DeltaTime * 1.25f);

		SetActorRotation(playerRot);
	}
	else {
		FRotator playerRot = GetActorRotation();
		playerRot.Pitch = FMath::Lerp(GetActorRotation().Pitch, currentRotation.Pitch, speedOfRotation_ * DeltaTime * 1.25f);
		playerRot.Roll = FMath::Lerp(GetActorRotation().Roll, currentRotation.Roll, speedOfRotation_ * DeltaTime * 1.25f);

		SetActorRotation(playerRot);
	}
}

void AMyShip::RotateShip(float DeltaTime)
{
	FHitResult hit;
	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(this);

	FVector startFront = hoverPoint1_->GetComponentLocation();
	FVector endFront = startFront - FVector(0.0f, 0.0f, desiredHeight_ * 1000.0f);

	FVector A;
	FVector B;
	FVector C;

	if (debug)DrawDebugLine(GetWorld(), startFront, endFront, FColor::Green, false, -1.0f);
	if (GetWorld()->LineTraceSingleByChannel(hit, startFront, endFront, ECC_Visibility, traceParams)) {
		A = hit.ImpactPoint + (FVector::UpVector * desiredHeight_);
		if (debug)DrawDebugSphere(GetWorld(), A, 100.0f, 32, FColor::Red);
	}

	FVector startRight = hoverPoint2_->GetComponentLocation();
	FVector endRight = startRight - FVector(0.0f, 0.0f, desiredHeight_ * 1000.0f);

	FHitResult hit1;
	if (debug)DrawDebugLine(GetWorld(), startRight, endRight, FColor::Green, false, -1.0f);
	if (GetWorld()->LineTraceSingleByChannel(hit1, startRight, endRight, ECC_Visibility, traceParams)) {
		B = hit1.ImpactPoint + (FVector::UpVector * desiredHeight_);
		if (debug)DrawDebugSphere(GetWorld(), B, 100.0f, 32, FColor::Red);
	}

	FVector startLeft = hoverPoint3_->GetComponentLocation();
	FVector endLeft = startLeft - FVector(0.0f, 0.0f, desiredHeight_ * 1000.0f);

	FHitResult hit2;
	if (debug)DrawDebugLine(GetWorld(), startLeft, endLeft, FColor::Green, false, -1.0f);
	if (GetWorld()->LineTraceSingleByChannel(hit2, startLeft, endLeft, ECC_Visibility, traceParams)) {
		C = hit2.ImpactPoint + (FVector::UpVector * desiredHeight_);
		if (debug)DrawDebugSphere(GetWorld(), C, 100.0f, 32, FColor::Red);
	}

	FVector center;
	center.X = (A.X + B.X + C.X) / 3.0f;
	center.Y = (A.Y + B.Y + C.Y) / 3.0f;
	center.Z = (A.Z + B.Z + C.Z) / 3.0f;

	FVector AC = C - A;
	AC.Normalize();

	FVector AB = B - A;
	AB.Normalize();

	FVector cross = FVector::CrossProduct(AB, AC);

	center.Z += (FMath::Cos(timePast_ * levitateSpeed_)) * levitateHeight_;

	if (debug)DrawDebugSphere(GetWorld(), center, 100.0f, 32, FColor::Yellow);
	if (debug)DrawDebugDirectionalArrow(GetWorld(), center, center + (cross * 500.0f), 5000.0f, FColor::Yellow, false, -1.0f, (uint8)0U, 20.0f);

	timePast_ += DeltaTime;
	if ((timePast_ + 10.0f) > MAX_FLT) {
		timePast_ = 0.0f;
	}

	FVector resultingPos;
	if (center.Z >= GetActorLocation().Z) {
		resultingPos = FMath::Lerp(GetActorLocation(), FVector(GetActorLocation().X, GetActorLocation().Y, center.Z), (DeltaTime * speedOfGoingToHeight_));
	}
	else {
		resultingPos = FMath::Lerp(GetActorLocation(), FVector(GetActorLocation().X, GetActorLocation().Y, center.Z), (DeltaTime * speedOfGoingToHeight_) / (FVector::Dist(hit.ImpactPoint, A) / gravitySpeed_));
	}
	SetActorLocation(resultingPos);

	FVector forward = A - center;
	forward.Normalize();

	if (debug)DrawDebugDirectionalArrow(GetWorld(), center, center + (forward * 500.0f), 5000.0f, FColor::Yellow, false, -1.0f, (uint8)0U, 20.0f);

	FRotator rotation = UKismetMathLibrary::MakeRotFromXZ(forward, cross);

	currentRotation = rotation;
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
	AddMovementInput(GetActorForwardVector(), input);
}

void AMyShip::MoveCameraX(float input)
{
	springArm_->AddRelativeRotation(FRotator(0, input, 0));
}

// Called every frame
void AMyShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveShip(DeltaTime);

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
	}
}


