// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "CustomObject.h"
#include "CustomGameMode.h"
#include "CustomARPawn.h"
#include "CustomGameState.h"
#include "Hatch.generated.h"

/**
 *
 */
UCLASS()
class UE5_AR_API AHatch : public ACustomObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHatch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TimerManager();

	ACustomGameMode* CustomGameMode;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DoorFrame;

	ACustomGameState* GS;
	ACustomARPawn* Player;
};
