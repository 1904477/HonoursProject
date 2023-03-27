// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomObject.h"
#include "CustomGameMode.h"
#include "TableObstacle.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API ATableObstacle : public ACustomObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATableObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ACustomGameMode* CustomGameMode;
};
