

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class VROOMVROOMPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* playerOwner = nullptr;

	AItem();
	virtual void Tick(float DeltaTime) override;

	virtual void Execute() {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveUI();

	void RemoveUI_Implementation() {};

	virtual void SetPlayerOwner(AActor* pPlayerOwner) { playerOwner = pPlayerOwner; };

protected:
	virtual void BeginPlay() override;
	

};
