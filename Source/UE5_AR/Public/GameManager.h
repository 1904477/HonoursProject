// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "PlaceablePlayer.h"
#include "ARPlaneActor.h"
#include "GameManager.generated.h"

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

	/**
 * @brief A Dummy event for spawning ACustomActor.
 */
	UFUNCTION(BlueprintCallable, Category = "Spawn")
		virtual void SpawnCube();
	
	UFUNCTION(BlueprintCallable, Category = "Spawn")
		void SpawnEnemy();
	
 //* @brief Perform a line trace at the screen position provided and place a default Placeable actor on it.
 //* Create another actor class that inherits from APlaceableActor and is spawned here instead of the default one. Make sure this new object is placed on the Plane rather than halfway through a plane
 //* @param ScreenPos Pass the Screen Position as an FVector
 //*/

	FTransform LineTraceResult(FVector ScreenPos);
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void flipPlaneVisibility();

	
	UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
		APlaceablePlayer* Player;
	
	UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<APlaceableCharacter> PlacableToSpawn;

	UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
		TArray<APlaceablePlayer*> Enemies;

	FTransform TraceResultTransform;

	
	int EnemiesToSpawn =0;
};
