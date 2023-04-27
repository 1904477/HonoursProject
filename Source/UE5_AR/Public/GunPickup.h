// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomPickup.h"
#include "CustomGameState.h"
#include "GunPickup.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API AGunPickup : public ACustomPickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGunPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category = "GunComponent", EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SkeletalMesh;


	UPROPERTY(Category = "GunAnimation", VisibleAnywhere, BlueprintReadWrite)
		bool inShootingAnimation = false;

	ACustomGameState* GS;

	UPROPERTY(Category = "GunPicked", VisibleAnywhere, BlueprintReadWrite)
	bool isGunPicked = false;


};
