// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnedEnemy.h"
#include "PoissonSampler.h"
#include "TableObstacle.h"
#include "GameObjectsSpawner.generated.h"

class ACustomGameState;
class ACustomGameMode;
class AGameManager;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float EnemiesSpawnTimer = 0;

	UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<APlaceableCharacter> PlacableToSpawn;

	UPROPERTY(Category = "Enemies", EditAnywhere, BlueprintReadWrite)
		TArray<ASpawnedEnemy*> Enemies;

	TArray<ATableObstacle*> Tables;



	int EnemiesToSpawn = 0;
	float EnemySpawnTimer = 0;

	ACustomGameState* GameState;
	ACustomGameMode* CustomGameMode;
	UPoissonSampler* PoissonSampler;

	bool isComponentAdded = false;
};
