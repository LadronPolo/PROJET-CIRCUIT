#include "MysteryBox.h"
#include "Vehicle.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Components/BoxComponent.h"
#include <VroomVroomProject/Public/AbilityPawn.h>

// Sets default values
AMysteryBox::AMysteryBox()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(Mesh);
}

void AMysteryBox::BeginPlay()
{
	Super::BeginPlay();

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AMysteryBox::BoxPickUp);
	
	TArray<EItem> FirstPlaceDropChance;
	for (int i = 0; i < 99; i++)
	{
		FirstPlaceDropChance.Add(EItem::EI_GreenShell);
	}
	for (int i = 0; i < 1; i++)
	{
		FirstPlaceDropChance.Add(EItem::EI_Star);
	}
	ItemDropChanceByPlace.Add(FirstPlaceDropChance);
}

void AMysteryBox::BoxPickUp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	IAbilityPawn* ability = Cast<IAbilityPawn>(OtherActor);
	if (ability)
	{
		if (ability->item == nullptr)
			PickupItem(OtherActor);

		if(Box)
		Box->SetVisibility(false);
		BoxCollider->SetGenerateOverlapEvents(false);
		BoxCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetWorldTimerManager().SetTimer(BoxResetTimer, this, &AMysteryBox::ResetBox, ResetTimer);
	}
}

void AMysteryBox::ResetBox()
{
	if (Box)
	Box->SetVisibility(true);
	BoxCollider->SetGenerateOverlapEvents(true);
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMysteryBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

