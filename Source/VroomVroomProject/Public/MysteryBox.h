#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemTypes.h"
#include "Item.h"
#include "MysteryBox.generated.h"

UCLASS()
class VROOMVROOMPROJECT_API AMysteryBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AMysteryBox();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float ResetTimer = 7.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollider;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void BoxPickUp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ResetBox();

	FTimerHandle BoxResetTimer;
	TArray<TArray<EItem>> ItemDropChanceByPlace;

	UPROPERTY(EditAnywhere, category="item")
	TArray<TSubclassOf<AItem>> items;
};
