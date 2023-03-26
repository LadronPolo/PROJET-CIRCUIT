// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilityPawn.h"
#include "car.generated.h"

UCLASS()
class VROOMVROOMPROJECT_API Acar : public APawn, public IAbilityPawn
{
	GENERATED_BODY()

public:
	Acar();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void PickItem_Implementation(TSubclassOf<AItem> pItem) override;

	void FreezeInput_Implementation(float duration) override;

	void RestoreInput_Implementation() override;

	void Boost_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void useItem();


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

};
