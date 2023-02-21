// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "PoissonSampler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_AR_API UPoissonSampler : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPoissonSampler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<FVector2f> GeneratePoisson(float minDistMainPoints,float minDistSecPoints, int new_points_count, int secondary_points);

	bool inNeighbourhood(FVector2f point, float mindist);

	
	TArray<FVector2f>MainPoints;
	TArray<FVector2f>SecondaryPoints;

protected:
	UNavigationSystemV1* NavigationArea;
};
