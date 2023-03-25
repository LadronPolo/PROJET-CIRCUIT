

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RacingLine.generated.h"

UCLASS()
class VROOMVROOMPROJECT_API ARacingLine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARacingLine();

	UFUNCTION(BlueprintCallable)
	void Initialize(class UInstancedStaticMeshComponent* Mesh, class UStaticMesh* NewMesh, class USplineComponent* Spline, float Step);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	class USplineComponent* SplineComponent;
};
