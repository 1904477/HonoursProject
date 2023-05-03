// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "CustomGameMode.h"
#include "GameManager.h"

#include "CustomARPawn.generated.h"
class UCameraComponent;
class UCapsuleComponent;
class ACustomGameMode;
class AGunPickup;

UCLASS()
class UE5_AR_API ACustomARPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACustomARPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnScreenTouch(const ETouchIndex::Type FingerIndex, const FVector ScreenPos);
	virtual void OnScreenRelease(const ETouchIndex::Type FingerIndex, const FVector ScreenPos);

	UFUNCTION(Category = "Shooting", BlueprintCallable)
	void Shoot();

	bool WorldHitTest( FHitResult& fHit);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Category = "SceneComponent", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;
	
	UPROPERTY(Category = "CameraComponent", VisibleAnywhere, BlueprintReadWrite)
		UCameraComponent* CameraComponent;

	UPROPERTY(Category = "CapsuleComponent", VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;

	//ACustomGameMode* GM;
	UPROPERTY(Category = "GameManager", VisibleAnywhere, BlueprintReadWrite)
	AGameManager* GameManager;
	
	ACustomGameState* GS;

	AGunPickup* Gun;

	UPROPERTY(Category = "camLocation", VisibleAnywhere, BlueprintReadWrite)
	FVector camLocation;

	UPROPERTY(Category = "DistanceToEnemy", VisibleAnywhere, BlueprintReadWrite)
		FVector DistanceToClosestEnemy;

	UPROPERTY(Category = "CameraManager", VisibleAnywhere, BlueprintReadOnly)
	APlayerCameraManager* camManager;
};
