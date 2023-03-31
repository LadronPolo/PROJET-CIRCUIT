#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.h"
#include "GreenShell.generated.h"

UCLASS()
class VROOMVROOMPROJECT_API AGreenShell : public AItem
{
	GENERATED_BODY()
	
public:
	AGreenShell();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	bool InWorld = false;

	UPROPERTY(EditInstanceOnly)
	float LifeSpan = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Speed = 7000.f;

	UPROPERTY(EditDefaultsOnly)
	float freezeDuration = 2.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	void Execute_Implementation() override;

	void SetPlayerOwner(AActor* pPlayerOwner) override;

protected:
	virtual void BeginPlay() override;	

private:
	UFUNCTION()
	void ShellHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
