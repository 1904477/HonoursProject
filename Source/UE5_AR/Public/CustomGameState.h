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
	

	UFUNCTION(BlueprintCallable, Category = "Score")
		void SetScore(const int32 NewScore);

	void SetPlaceableSelected(bool value);
	void SetAreARPlanesDisplayed(bool value);

	bool GetPlaceableSelected();
	bool GetAreARPlanesDisplayed();

	UFUNCTION(BlueprintCallable, Category = "Score")
		int32 GetScore();

	bool IsPlaceableSelected;
	bool IsPlaceableSpawned;
	int Score;

	UPROPERTY(Category = "ARPlanesRender", EditAnywhere, BlueprintReadWrite)
	bool AreARPlanesDisplayed;
};
