

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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Execute();

	virtual void Execute_Implementation() {};

	virtual void SetPlayerOwner(AActor* pPlayerOwner) { playerOwner = pPlayerOwner; };

protected:
	virtual void BeginPlay() override;
	

};
