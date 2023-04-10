// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceableCharacter.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "CustomGameState.h"
#include "SpawnedEnemy.generated.h"
class ACustomGameMode;
class ACustomARPawn;
//class AGameManager;

UENUM()
enum Status
{
	Idle     UMETA(DisplayName = "Idle"),
	Wandering      UMETA(DisplayName = "Wandering"),
	Suspicious   UMETA(DisplayName = "Suspicious"),
	Charging   UMETA(DisplayName = "Charging"),
	Attacking   UMETA(DisplayName = "Attacking"),
	Spawning   UMETA(DisplayName = "Spawning"),
};
UCLASS()
class UE5_AR_API ASpawnedEnemy : public APlaceableCharacter
{
	GENERATED_BODY()
public:
	ASpawnedEnemy();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void EnemySuspicious();

	void EnemyStatusManager();

	void EnemyWander();

	void EnemyCharging();

	void ClosestObstacleChecker();

	void FallDetection();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStatus")
	TEnumAsByte<Status> EnemyStatus;

	float EnemyStatusTimer = 0;
	float WanderRadius;
	float SpawnRadius = 500;
	float SuspiciousTimer = 0;
	float StateSwitchTimer = 0;
	float SuspiciousDistance = 0;
	float ChargeDistance = 0;
	float Health = 100;

protected:
	AAIController* AIController;
	UNavigationSystemV1* NavigationArea;
	ACustomARPawn* Player;
};
