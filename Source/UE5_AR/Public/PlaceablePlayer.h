// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaceableCharacter.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "PlaceablePlayer.generated.h"
class ACustomGameMode;
//class AGameManager;

UENUM()
enum Status
{
	Idle     UMETA(DisplayName = "Idle"),
	Wandering      UMETA(DisplayName = "Wandering"),
	Suspicious   UMETA(DisplayName = "Suspicious"),
};
UCLASS()
class UE5_AR_API APlaceablePlayer : public APlaceableCharacter
{
	GENERATED_BODY()
public:
	APlaceablePlayer();
	
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
	AGameManager* GameManager;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStatus")
	TEnumAsByte<Status> EnemyStatus;


	float EnemyStatusTimer = 0;
	FVector MoveTo;
	float WanderRadius;
	float SpawnRadius = 500;
	float SuspiciousTimer = 0;

	FColor BoxColor;

protected:
	AAIController* AIController;
	UNavigationSystemV1* NavigationArea;
};
