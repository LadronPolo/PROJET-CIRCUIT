#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckPoint.generated.h"

UCLASS()
class VROOMVROOMPROJECT_API ACheckPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int number = -1;

	ACheckPoint();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
