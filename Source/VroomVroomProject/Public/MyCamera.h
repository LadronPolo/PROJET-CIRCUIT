// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SplineComponent.h"
#include "MyCamera.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VROOMVROOMPROJECT_API UMyCamera : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyCamera();

	UPROPERTY(EditAnywhere, Category = "Stats")
		USpringArmComponent* springArm_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		UCameraComponent* cameraComponent_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		USplineComponent* spline_;

	UPROPERTY(EditAnywhere, Category = "Stats")
		APawn* ship_;

	float distanceToPlayer_;
	FVector worldLocation_;
	FVector playerForwardVector_;
	FVector playerLocation_;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CameraFollowsSpline();
		
};
