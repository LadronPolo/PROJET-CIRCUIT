// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SplineComponent.h"
#include "MyShip.generated.h"

UCLASS()
class VROOMVROOMPROJECT_API AMyShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyShip();

	UPROPERTY(EditAnywhere, Category = "Stats")
		UStaticMeshComponent* shipMesh_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		UStaticMeshComponent* hoverPoint1_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		UStaticMeshComponent* hoverPoint2_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		UStaticMeshComponent* hoverPoint3_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		USpringArmComponent* springArm_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		UCameraComponent* cameraComponent_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		UFloatingPawnMovement* floatingPawnMovement_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		USplineComponent* spline_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float speedOfRotation_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float desiredHeight_ = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float levitateHeight_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float levitateSpeed_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float speedOfGoingToHeight_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float gravitySpeed_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		bool debug = true;

	float timePast_;

	FVector lastRotation_;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveShip(float DeltaTime);
	void ForwardAxis(float input);
	void SideAxis(float input);
	void Accelerate(float input);

	void MoveCameraX(float input);
	void RotateShip(float DeltaTime);

	void StartDrift();
	void StopDrift();

	void CameraLookAtPlayer();

	void CameraFollowsSpline();

	FVector lastShipPosition_;
	bool justDetached_ = false;

	float axisX_;
	float axisY_;
	float accelInput_;
	bool  isDrifiting_ = false;
	float originalTurningBoost_;
	float originalDeceleration_;
	float speedMultiplier_;

	bool isFalling_ = false;

	float deltaTime_;

	float angleAxis_ = 0.0f;

	FVector green_;
	FVector blue_;
	FVector red_;

	FVector yellow_;


	FVector blackVector_;
	FRotator currentRotation;

	float distanceToPlayer_;
	FVector worldLocation_;
	FVector playerForwardVector_;
	FVector playerLocation_;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
