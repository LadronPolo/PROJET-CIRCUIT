// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCamera.h"

// Sets default values for this component's properties
UMyCamera::UMyCamera()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	ship_ = (APawn*)GetOwner();
	
	springArm_ = CreateDefaultSubobject<USpringArmComponent>("SpringArm");

	cameraComponent_ = CreateDefaultSubobject<UCameraComponent>("Camera");
	cameraComponent_->SetupAttachment(springArm_);

	distanceToPlayer_ = 10.0f;
}


// Called when the game starts
void UMyCamera::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMyCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UMyCamera::CameraFollowsSpline()
{
	worldLocation_ = ship_->GetActorLocation() + ship_->GetActorForwardVector() * distanceToPlayer_;
	
	FVector cameraPos = spline_->FindLocationClosestToWorldLocation(worldLocation_, ESplineCoordinateSpace::World);
	springArm_->SetWorldLocation(cameraPos);
}

