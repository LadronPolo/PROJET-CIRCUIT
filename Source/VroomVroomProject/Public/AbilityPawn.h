#pragma once

#include "Item.h"
#include "AbilityPawn.generated.h"

/**
 * Must have BlueprintType as a specifier to have this interface exposed to blueprints.
 * With this line you can easily add this interface to any blueprint class.
 */
UINTERFACE(MinimalAPI)
class UAbilityPawn : public UInterface {
	GENERATED_BODY()
};

class VROOMVROOMPROJECT_API IAbilityPawn {
	GENERATED_BODY()

public:
	float EnergyMax = 100.f;
	float EnergyRemaining = 0.f;
	float EnergyChargeRate = 2.f;
	float EnergyNeededToBoost = 40.f;
	float BoostCost = 4.f;
	bool BoostAvaillable = false;
	bool IsBoosting = false;

	AItem* item = nullptr;
	FTimerHandle inputDisabled;

	FTimerHandle invincibleTimer;
	bool invincible = false;


	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void PickItem(TSubclassOf<AItem> pItem);

	virtual void PickItem_Implementation(TSubclassOf<AItem> pItem) = 0;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void FreezeInput(float duration);

	virtual void FreezeInput_Implementation(float duration) = 0;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void RestoreInput();

	virtual void RestoreInput_Implementation() = 0;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void SetInvincible(float duration);

	virtual void SetInvincible_Implementation(float duration) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void DisableInvincible();

	virtual void DisableInvincible_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void UseItem();

	virtual void UseItem_Implementation() = 0;
};
