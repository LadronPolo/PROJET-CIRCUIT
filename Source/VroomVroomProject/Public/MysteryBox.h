#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemTypes.h"
#include "Item.h"
#include "AbilityPawn.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Box;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollider;

	UFUNCTION(BlueprintNativeEvent, Category = "")
	void PickupItem(AActor* OtherActor);
	virtual void PickupItem_Implementation(AActor* OtherActor) {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "item")
	TArray<TSubclassOf<AItem>> items;
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void BoxPickUp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ResetBox();

	FTimerHandle BoxResetTimer;
	TArray<TArray<EItem>> ItemDropChanceByPlace;
};
