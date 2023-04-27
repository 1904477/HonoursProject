// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CustomGameState.generated.h"

/**
 * 
 */

UCLASS()
class UE5_AR_API ACustomGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ACustomGameState();
	~ACustomGameState() = default;

	UFUNCTION(Category = "GameReset", BlueprintCallable)
		void ResetVariables();

	UFUNCTION(Category = "ARPlanesRender", BlueprintCallable)
	void SetAreARPlanesDisplayed(bool value);
	UFUNCTION(Category = "ARPlanesRender", BlueprintCallable)
	bool GetAreARPlanesDisplayed();
	UFUNCTION(Category = "GameStart", BlueprintCallable)
	bool GetHasGameStarted();
	UFUNCTION(Category = "GameStart", BlueprintCallable)
	void SetHasGameStarted(bool b);
	UFUNCTION(Category = "EnvironmentScanned", BlueprintCallable)
	bool GetIsEnvironmentScanned();
	UFUNCTION(Category = "EnvironmentScanned", BlueprintCallable)
	void SetIsEnvironmentScanned(bool b);
	UFUNCTION(Category = "GunCollected", BlueprintCallable)
	bool GetIsIsGunCollected();
	UFUNCTION(Category = "GunCollected", BlueprintCallable)
	void SetIsIsGunCollected(bool b);
	UFUNCTION(Category = "Health", BlueprintCallable)
	void SetHealth(int b);
	UFUNCTION(Category = "Health", BlueprintCallable)
	int GetHealth();
	UFUNCTION(Category = "Ammo", BlueprintCallable)
	void SetAmmo(int b);
	UFUNCTION(Category = "Ammo", BlueprintCallable)
	int GetAmmo();
	UFUNCTION(Category = "Ammo", BlueprintCallable)
	void SetAmmoReload(int b);
	UFUNCTION(Category = "Ammo", BlueprintCallable)
	int GetAmmoReload();
	UFUNCTION(Category = "Score", BlueprintCallable)
	void SetScore(int b);
	UFUNCTION(Category = "Score", BlueprintCallable)
	int GetScore();
	UFUNCTION(Category = "EnemyAlive", BlueprintCallable)
	bool GetIsIsIsOneEnemyAlive();
	UFUNCTION(Category = "EnemyAlive", BlueprintCallable)
	void SetIsIsIsOneEnemyAlive(bool b);
	UFUNCTION(Category = "EnemyClose", BlueprintCallable)
	bool GetIsEnemyTooClose();
	UFUNCTION(Category = "EnemyClose", BlueprintCallable)
	void SetIsEnemyTooClose(bool b);
	UFUNCTION(Category = "HatchOpen", BlueprintCallable)
		bool GetIsHatchOpen();
	UFUNCTION(Category = "HatchOpen", BlueprintCallable)
		void SetIsHatchOpen(bool b);

	UFUNCTION(Category = "DataCollect", BlueprintCallable)
		void SetTableCollisions(int b);
	UFUNCTION(Category = "DataCollect", BlueprintCallable)
		int GetTableCollisions();

	UFUNCTION(Category = "HatchOpenTimer", BlueprintCallable)
		void SetHatchOpenTimer(float b,float t);
	UFUNCTION(Category = "HatchOpenTimer", BlueprintCallable)
		float GetHatchOpenTimerMin();
	UFUNCTION(Category = "HatchOpenTimer", BlueprintCallable)
		float GetHatchOpenTimerSec();

	UFUNCTION(Category = "PlayerWin", BlueprintCallable)
		bool GetDidPlayerWin();
	UFUNCTION(Category = "PlayerWin", BlueprintCallable)
		void SetDidPlayerWin(bool b);
	UFUNCTION(Category = "PlayerLose", BlueprintCallable)
		bool GetDidPlayerLose();
	UFUNCTION(Category = "PlayerLose", BlueprintCallable)
		void SetDidPlayerLose(bool b);


	
private:
		bool HasGameStarted;
		bool IsEnvironmentScanned;
		bool AreARPlanesDisplayed;
		bool IsGunCollected;
		bool IsOneEnemyAlive;
		bool IsEnemyTooClose;
		bool DidPlayerWin;
		bool DidPlayerLose;
		bool IsHatchOpen;
		int Health;
		int AmmoNumber;
		int ReloadNumber;
		int Score;
		int TableCollision;
		float HatchOpenTimerMin;
		float HatchOpenTimerSec;
};
