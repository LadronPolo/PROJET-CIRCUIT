
#include "car.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

Acar::Acar()
{
	PrimaryActorTick.bCanEverTick = true;
	EnergyChargeRate = 0;
}

void Acar::BeginPlay()
{
	Super::BeginPlay();
	
}

void Acar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EnergyRemaining = EnergyRemaining < EnergyMax ? (EnergyRemaining + DeltaTime * EnergyChargeRate) : EnergyMax;
	BoostAvaillable = EnergyRemaining >= EnergyNeededToBoost;
}

void Acar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
}

void Acar::PickItem_Implementation(TSubclassOf<AItem> pItem)
{
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	if (!item)
	{
		item = Cast<AItem>(GetWorld()->SpawnActor(pItem.Get(), &Location, &Rotation, SpawnParams));
		item->SetPlayerOwner(this);
	}
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

void Acar::nullifyItem()
{
	item = nullptr;
}