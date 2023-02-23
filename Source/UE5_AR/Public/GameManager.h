// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "CustomGameState.h"
#include "GameManager.generated.h"

class AGameObjectsSpawner;

UCLASS()
class UE5_AR_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Spawner")	//Function to spawn single enemy.
		void SpawnGameObjectsSpawner();

	UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AGameObjectsSpawner> GameObjectsSpawnerToSpawn;

	UPROPERTY(Category = "NumberOfEnemiesSpawned", EditAnywhere, BlueprintReadWrite)
		int EnemiesToSpawn;

	UPROPERTY(Category = "EnemiesSpawnTimer", EditAnywhere, BlueprintReadWrite)
		float EnemiesSpawnTimer;
	
	UPROPERTY(Category = "EnemiesSuspiciousDistance", EditAnywhere, BlueprintReadWrite)
		float EnemiesSuspiciousDistance;

	UPROPERTY(Category = "MainPointsSpawnNumber", EditAnywhere, BlueprintReadWrite)
		int MainPointsSpawnNum;
	UPROPERTY(Category = "SecondaryPointsSpawnNumber", EditAnywhere, BlueprintReadWrite)
		int SecPointsSpawnNum;
	UPROPERTY(Category = "MainPointsMinDistance", EditAnywhere, BlueprintReadWrite)
		float MainPointsMinDist;
	UPROPERTY(Category = "SecondaryPointsMaximumDistance", EditAnywhere, BlueprintReadWrite)
		float SecPointsMaxDist;
 //* @brief Perform a line trace at the screen position provided and place a default Placeable actor on it.
 //* Create another actor class that inherits from APlaceableActor and is spawned here instead of the default one. Make sure this new object is placed on the Plane rather than halfway through a plane
 //* @param ScreenPos Pass the Screen Position as an FVector
 //*/

	FTransform LineTraceResult(FVector ScreenPos);
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void flipPlaneVisibility();



	FTransform TraceResultTransform;
	AGameObjectsSpawner* ObjectsSpawner;
	ACustomGameState* GameState;

	bool hasFinishedScanning = false;
};
