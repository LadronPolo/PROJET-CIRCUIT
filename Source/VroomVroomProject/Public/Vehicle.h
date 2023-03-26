#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "ItemTypes.h"
#include "AbilityPawn.h"
#include "Vehicle.generated.h"

/**
 * 
 */
UCLASS()
class VROOMVROOMPROJECT_API AVehicle : public AWheeledVehiclePawn, public IAbilityPawn
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		bool IsHitByItem = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool BoostAvailable = false;

	UPROPERTY(BlueprintReadWrite)
		EItem HoldItem = EItem::EI_None;

	UPROPERTY(EditInstanceOnly)
		float ItemUsageMinTime = 5.f;

	UPROPERTY(EditInstanceOnly)
		float ItemUsageMaxTime = 15.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool SpawnShell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EnergyMax = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EnergyRemaining = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EnergyChargeRate = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EnergyNeededToBoost = 40.f;

	UPROPERTY(BlueprintReadWrite)
		USceneComponent* ProjectileShootingPoint = nullptr;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AGreenShell> Shell;

	virtual void Tick(float DeltaTime) override;
	
private:
	FTimerHandle HitResetTimer;
	FTimerHandle GreenShellResetTimer;

	bool IsHitDoOnce = true;
	bool ItemAcquired = true;

	void RestartHit();
	void ThrowGreenShell();


	void PickItem_Implementation(TSubclassOf<AItem> pItem) override;

	void FreezeInput_Implementation(float duration) override;

	void RestoreInput_Implementation() override;

	void SetInvincible_Implementation(float duration) override {};

	void DisableInvincible_Implementation() override {};

	void UseItem_Implementation() override;

	void SetVelocity_Implementation(FVector value) override {};

	void AddImpule_Implementation(FVector value) override {};

	void Miniaturize_Implementation(float duration) override {};

	void ResetMiniaturize_Implementation() override {};

	void AddEnergy_Implementation(float energy) override {};
};
