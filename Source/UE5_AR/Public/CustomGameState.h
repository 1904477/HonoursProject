// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CustomGameState.generated.h"

/**
 * 
 */

UENUM()
enum SessionMode
{
	VirtualObstacles     UMETA(DisplayName = "VirtualObstacles"),
	RealObstacles      UMETA(DisplayName = "RealObstacles"),
};
UCLASS()
class UE5_AR_API ACustomGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ACustomGameState();
	~ACustomGameState() = default;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionMode")
		TEnumAsByte<SessionMode> SessionModeSelected;

private:
		bool HasGameStarted;
		bool IsEnvironmentScanned;
		bool AreARPlanesDisplayed;
		bool IsGunCollected;


};
