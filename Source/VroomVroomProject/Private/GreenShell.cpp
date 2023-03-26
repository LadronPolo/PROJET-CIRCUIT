#include "GreenShell.h"
#include "Vehicle.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

AGreenShell::AGreenShell()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->IgnoreActorWhenMoving(GetOwner(), true);
		Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
		Mesh->SetGenerateOverlapEvents(true);
	}

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->InitialSpeed = Speed;
		ProjectileMovementComponent->MaxSpeed = 0.f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.f;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 1.f;
		ProjectileMovementComponent->Friction = 0.f;
		ProjectileMovementComponent->bSimulationEnabled = false;
	}
}

void AGreenShell::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AGreenShell::ShellHit);
}

void AGreenShell::ShellHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (playerOwner == nullptr)
		return;

	IAbilityPawn* ability = Cast<IAbilityPawn>(OtherActor);
	if (ability && playerOwner != OtherActor)
	{
		IAbilityPawn::Execute_FreezeInput(OtherActor, freezeDuration);
		Destroy();
	}
}

void AGreenShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGreenShell::Execute_Implementation()
{	
	FVector vector = playerOwner->GetActorForwardVector();
	vector.Z = 0.f;

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ProjectileMovementComponent->bSimulationEnabled = true;
	ProjectileMovementComponent->Velocity = vector * Speed;
	SetLifeSpan(LifeSpan);

	//Mesh->SetSimulatePhysics(true);
};


void AGreenShell::SetPlayerOwner(AActor* pPlayerOwner)
{
	Super::SetPlayerOwner(pPlayerOwner);
	AttachToActor(pPlayerOwner, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Shell");
}