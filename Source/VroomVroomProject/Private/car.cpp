
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

	if (IsBoosting)
	{
		EnergyRemaining -= DeltaTime * BoostCost;
		if (EnergyRemaining <= 0)
		{
			EnergyRemaining = 0;
			IsBoosting = false;
		}
	}
	else
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

	item = Cast<AItem>(GetWorld()->SpawnActor(pItem.Get(), &Location, &Rotation, SpawnParams));
	item->SetPlayerOwner(this);
}

void Acar::FreezeInput_Implementation(float duration)
{
	if (invincible)
		return;

	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	GetWorldTimerManager().SetTimer(inputDisabled, this, &Acar::RestoreInput_Implementation, duration);
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