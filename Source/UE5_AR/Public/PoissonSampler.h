// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "PoissonSampler.generated.h"

class ACustomARPawn;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )		//Component class that uses the Poisson Disc Sampling algorithm to generate random points equally distributed in the game.
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

	TArray<FVector2f> GeneratePoisson(float minDistMainPoints,float minDistSecPoints, int new_points_count, int secondary_points, float distToPlayer);		//Function that generates the points using the P. algorithm.

	bool inNeighbourhood(FVector2f point, float mindist, float distToPlayer);		//Function that checks if the point passed is close to other points.

	
	TArray<FVector2f>MainPoints;		//Vector of main points.
	TArray<FVector2f>SecondaryPoints;	//Vector of secondary points.

protected:
	UNavigationSystemV1* NavigationArea;	//Navigation area, necessary for distributing the points in the navmesh.
	ACustomARPawn* Player;			//Player reference to generate main points.
};
