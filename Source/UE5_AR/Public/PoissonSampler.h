// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "PoissonSampler.generated.h"

class ACustomARPawn;
class ACustomGameMode;
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

	FVector MainPointsGeneration(float minDistMainPoints, int new_points_count, float distToPlayer, std::string pointType);

	void SecondaryPointsGeneration(float minDistSecPoints, int secondary_points);

	TArray<FVector> GeneratePoisson(float minDistMainPoints,float minDistSecPoints, int new_points_count, int secondary_points, float distToPlayer);		//Function that generates the points using the P. algorithm.

	bool inNeighbourhood(FVector point, float mindist, float distToPlayer);		//Function that checks if the point passed is close to other points.

	
	TArray<FVector>MainPoints;		//Vector of main points.
	TArray<FVector>SecondaryPoints;	//Vector of secondary points.
	UNavigationSystemV1* NavigationArea;	//Navigation area, necessary for distributing the points in the navmesh.
	UNavigationSystemV1* NavigationAreaSec;	//Navigation area, necessary for distributing the points in the navmesh.

protected:
	ACustomARPawn* Player;					//Player reference to generate main points.
	ACustomGameMode* CustomGameMode;			//Gamemode to obtain AR manager and lowest plane position.

	bool IsSecondaryPointClose;    
	bool IsSecondaryPointHigh;
	bool isMainPointClose;
	bool isMainPointHigh;

};
