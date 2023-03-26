#include "Vehicle.h"
#include "GreenShell.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (HoldItem == EItem::EI_GreenShell && ItemAcquired)
	{
		UKismetSystemLibrary::PrintString(this, "GreenShell Acquired!");
		ItemAcquired = false;
		float RandItemUsageTime = UKismetMathLibrary::RandomFloatInRange(ItemUsageMinTime, ItemUsageMaxTime);
		GetWorldTimerManager().SetTimer(GreenShellResetTimer, this, &AVehicle::UseItem_Implementation, RandItemUsageTime);
	}
	else if (HoldItem == EItem::EI_GreenShell)
	{

	}

	if (EnergyRemaining < EnergyMax)
	{
		EnergyRemaining += DeltaTime * EnergyChargeRate;
	}
	else
	{
		EnergyRemaining = EnergyMax;
	}

	if (EnergyRemaining > EnergyNeededToBoost)
	{
		BoostAvailable = true;
	}
	else 
	{
		BoostAvailable = false;
	}
}

void AVehicle::RestartHit()
{
	IsHitByItem = false;
	IsHitDoOnce = true;
}

void AVehicle::ThrowGreenShell()
{
	ItemAcquired = true;
	HoldItem = EItem::EI_None;
	if (ProjectileShootingPoint)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		GetWorld()->SpawnActor(Shell.Get(), &ProjectileShootingPoint->GetComponentTransform(), SpawnParams);
	}
	SpawnShell = true;
}


void AVehicle::PickItem_Implementation(TSubclassOf<AItem> pItem)
{
	FVector Location = GetActorLocation();
	FRotator Rotation = FRotator(0.f, 0.f, 0.f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	item = Cast<AItem>(GetWorld()->SpawnActor(pItem.Get(), &Location, &Rotation, SpawnParams));
	item->SetPlayerOwner(this);
}

void AVehicle::UseItem_Implementation()
{
	ItemAcquired = true;
	HoldItem = EItem::EI_None;
	if (ProjectileShootingPoint)
	{
		if (item)
		{
			item->Execute();
			item = nullptr;
		}

		/*FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		GetWorld()->SpawnActor(Shell.Get(), &ProjectileShootingPoint->GetComponentTransform(), SpawnParams);*/
	}
	SpawnShell = true;
}

void AVehicle::FreezeInput_Implementation(float duration)
{
	if (invincible)
		return;

	IsHitByItem = true;
	GetWorldTimerManager().SetTimer(inputDisabled, this, &AVehicle::RestoreInput_Implementation, duration);
}

void AVehicle::RestoreInput_Implementation()
{
	IsHitByItem = false;
}