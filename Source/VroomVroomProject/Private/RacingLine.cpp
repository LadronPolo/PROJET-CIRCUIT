#include "RacingLine.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARacingLine::ARacingLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ARacingLine::Initialize(UInstancedStaticMeshComponent* Mesh, UStaticMesh* NewMesh, USplineComponent* Spline, float Step)
{
	Mesh->SetStaticMesh(NewMesh);
	for (int i = 0; i < UKismetMathLibrary::FFloor(Spline->GetSplineLength() / Step); i++)
	{

	}
	SplineComponent = Spline;
}

// Called when the game starts or when spawned
void ARacingLine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARacingLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

