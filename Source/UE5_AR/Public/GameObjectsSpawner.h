// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnedEnemy.h"
#include "PoissonSampler.h"
#include "TableObstacle.h"
#include "StepObstacle.h"
#include "GraveObject.h"
#include "Hatch.h"
#include "CustomPickup.h"
#include "NavigationSystem.h"
#include "GameObjectsSpawner.generated.h"

class ACustomGameState;
class ACustomGameMode;
class AGameManager;
class ACustomARPawn;

UCLASS()
class UE5_AR_API AGameObjectsSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameObjectsSpawner();

	void EnemiesSpawner();
	void EnemiesSpawnerManager();
	void SpawnVirtualObstacles();
	void SpawnTables();
	void SpawnSteps();
	void SpawnGraves();
	void SpawnGun();
	void SpawnHatch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<APlaceableCharacter> EnemyToSpawn;
	UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ACustomPickup> GunToSpawn;
	UPROPERTY(Category = "Enemies", EditAnywhere, BlueprintReadWrite)
		TArray<ASpawnedEnemy*> Enemies;
	UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AHatch> HatchToSpawn;
	UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ATableObstacle> TableObstacleToSpawn;
	UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AStepObstacle> StepObstacleToSpawn;

	TArray<ATableObstacle*> Tables;
	TArray<AStepObstacle*> Steps;
	TArray<AGraveObject*> Graves;

	ACustomGameState* GameState;
	ACustomGameMode* CustomGameMode;
	UPoissonSampler* PoissonSampler;
	ACustomARPawn* Player;
	AHatch* Hatch;
	UNavigationSystemV1* NavigationArea;	//Navigation area, necessary for distributing the points in the navmesh.


	bool isComponentAdded = false;
	bool isGunSpawned = false;
	bool isHatchOnGround = false;
	bool isHatchSpawned = false;
	int	  EnemiesSpawned;
	float EnemySpawnTimer = 0;
	float EnemiesSpawnTimer = 0;
};
