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
	if (ability && ability->invincible && playerOwner != OtherActor)
	{
		IAbilityPawn::Execute_FreezeInput(OtherActor, freezeDuration);
		IAbilityPawn::Execute_SetVelocity(OtherActor, FVector(0, 0, 0));
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
	InWorld = true;
	//Mesh->SetSimulatePhysics(true);
};


void AGreenShell::SetPlayerOwner(AActor* pPlayerOwner)
{
	Super::SetPlayerOwner(pPlayerOwner);

	TArray<UActorComponent*> elements = pPlayerOwner->GetComponentsByClass(USkeletalMeshComponent::StaticClass());
	for (int i = 0; i < elements.Num(); i++)
	{
		USkeletalMeshComponent* meshCp = Cast<USkeletalMeshComponent>(elements[i]);
		if (meshCp && meshCp->GetSocketByName("Shell") != nullptr)
		{
			AttachToComponent(meshCp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Shell");
			return;
		}

	}
}