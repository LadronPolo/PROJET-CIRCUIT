// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilityPawn.h"
#include "CheckPoint.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "car.generated.h"


UCLASS()
class VROOMVROOMPROJECT_API Acar : public APawn, public IAbilityPawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		UStaticMeshComponent* shipMesh_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		UStaticMeshComponent* hoverPoint1_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		UStaticMeshComponent* hoverPoint2_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		UStaticMeshComponent* hoverPoint3_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		USpringArmComponent* springArm_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		UCameraComponent* cameraComponent_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		UFloatingPawnMovement* floatingPawnMovement_;

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

	FVector lastShipPosition_;
	bool justDetached_ = false;

	float axisY_;
	float accelInput_;
	bool  isDrifiting_ = false;
	float originalTurningBoost_;
	float originalDeceleration_;
	float speedMultiplier_;

	bool isFalling_ = false;

	float deltaTime_;

	float angleAxis_ = 0.0f;

	float timer_;
	float decelerationTimer_;
	float lastSpeedMax_;

	float boosterLoopSpeed_ = 1.2f;
	float boosterLoopCount_;
	float canGetDriftBoost_;

	bool isInBoost_;

	bool boost_;
	float boostAmount_;
	float boostTimer_;

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

	float speed_ = 0.0f;

	float currentForwardSpeed_;
	float currentLateralSpeed_;

	Acar();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override {};
	UFUNCTION(BlueprintCallable)
	void MoveShip(float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void RotateShip(float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void StartDrift();
	UFUNCTION(BlueprintCallable)
	void StopDrift();
	UFUNCTION(BlueprintCallable)
	void CameraLookAtPlayer();
	UFUNCTION(BlueprintCallable)
	void SideAxis(float input);
	UFUNCTION(BlueprintCallable)
	void Accelerate(float input);


	void PickItem_Implementation(TSubclassOf<AItem> pItem) override;

	void FreezeInput_Implementation(float duration) override;

	void ResetInputTrigger();

	void RestoreInput_Implementation() override;

	void SetInvincible_Implementation(float duration) override;

	void DisableInvincibleTrigger();

	void DisableInvincible_Implementation() override;

	void UseItem_Implementation() override;

	void SetVelocity_Implementation(FVector value) override {};

	void AddImpule_Implementation(FVector value) override {};

	void Miniaturize_Implementation(float duration) override;

	void ResetMiniaturizeTrigger();

	void ResetMiniaturize_Implementation() override;

	void AddEnergy_Implementation(float energy) override;

	UFUNCTION(BlueprintCallable)
	float getEnergyRemaining();

	UFUNCTION(BlueprintCallable)
	bool canBoost();

	UFUNCTION(BlueprintCallable)
	bool IsBoost();

	UFUNCTION(BlueprintCallable)
	void SetIsBoost(bool pState);

	UFUNCTION(BlueprintCallable)
	void SetEnergyChargeRate(float pValue);

	UFUNCTION(BlueprintCallable)
	float getEnergyChargeRate();

	UFUNCTION(BlueprintCallable)
	bool IsInvincible();

	UFUNCTION(BlueprintCallable)
	void spendBoost();

	UFUNCTION(BlueprintCallable)
	float getBoostCost();

	UFUNCTION(BlueprintCallable)
	void SetEnergyRemaining(float pValue);

	UFUNCTION(BlueprintCallable)
	static void sort(UPARAM(ref) TArray<ACheckPoint*>& checkpoints)
	{
		Algo::SortBy(checkpoints, &ACheckPoint::number, TLess<>());

	}

	UFUNCTION(BlueprintCallable)
	static float RangeLerp(float min, float max, float t)
	{
		return (1 / (max - min)) * (t - min);
	}
};
