
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
	if (invicible)
		return;

	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	GetWorldTimerManager().SetTimer(inputDisabled, this, &Acar::RestoreInput_Implementation, duration);
}

void Acar::RestoreInput_Implementation()
{
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void Acar::useItem()
{
	if (item)
	{
		item->Execute();
		item = nullptr;
	}
}

void Acar::Boost_Implementation()
{

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