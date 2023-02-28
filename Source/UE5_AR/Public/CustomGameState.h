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

	void SetPlaceableSelected(bool value);
	void SetAreARPlanesDisplayed(bool value);
	bool GetAreARPlanesDisplayed();

	bool GetHasGameStarted();
	void SetHasGameStarted(bool b);
	bool GetIsEnvironmentScanned();
	void SetIsEnvironmentScanned(bool b);
	bool GetIsIsGunCollected();
	void SetIsIsGunCollected(bool b);

	UPROPERTY(Category = "GameStart", EditAnywhere, BlueprintReadWrite)
		bool HasGameStarted;
	UPROPERTY(Category = "EnvironmentScanned", EditAnywhere, BlueprintReadWrite)
		bool IsEnvironmentScanned;
	UPROPERTY(Category = "ARPlanesRender", EditAnywhere, BlueprintReadWrite)
		bool AreARPlanesDisplayed;
	UPROPERTY(Category = "GunCollected", EditAnywhere, BlueprintReadWrite)
		bool IsGunCollected;
};
