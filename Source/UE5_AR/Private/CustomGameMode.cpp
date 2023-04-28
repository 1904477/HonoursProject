// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameMode.h"
#include "GameManager.h"
#include "CustomARPawn.h"
#include "CustomGameState.h"
#include "GameObjectsSpawner.h"
#include "HelloARManager.h"
#include "ARBlueprintLibrary.h"
#include "ARPlaneActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ACustomGameMode::ACustomGameMode()
{
	// Add this line to your code if you wish to use the Tick() function
	PrimaryActorTick.bCanEverTick = true;

	// Set the default pawn and gamestate to be our custom pawn and gamestate programatically
	DefaultPawnClass = ACustomARPawn::StaticClass();
	GameStateClass = ACustomGameState::StaticClass();

	//ConstructorHelpers::FObjectFinder<UARSessionConfig> ConfigAsset(TEXT("ARSessionConfig'/Game/Blueprints/HelloARSessionConfig.HelloARSessionConfig'"));
	//Config = ConfigAsset.Object;
}


void ACustomGameMode::StartPlay() 
{
	SpawnInitialActors();
	// This is called before BeginPlay
	StartPlayEvent();
	// This function will transcend to call BeginPlay on all the actors 
	Super::StartPlay();
}

// An implementation of the StartPlayEvent which can be triggered by calling StartPlayEvent() 
void ACustomGameMode::StartPlayEvent_Implementation() 
{
	// Start a timer which will call the SpawnCube Function every 4 seconds
}



void ACustomGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (UGameplayStatics::GetPlatformName() == "IOS" || UGameplayStatics::GetPlatformName() == "Android")
	{
		if (GetWorld()->GetMapName() == "VirtualObstaclesLevel" || GetWorld()->GetMapName() == "RealWorldObstaclesLevel")
		{
			if (isARSessionStarted == false)
			{
				UARBlueprintLibrary::StartARSession(Config);
				isARSessionStarted = true;
			}
		}
	}
}


void ACustomGameMode::SpawnInitialActors()
{
	// Spawn an instance of the HelloARManager class
	Config = NewObject<UARSessionConfig>();
	Config->bUseSceneDepthForOcclusion = true;
	Config->SetSessionTrackingFeatureToEnable(EARSessionTrackingFeature::SceneDepth);
	ARManager = GetWorld()->SpawnActor<AHelloARManager>();
	GameManager = GetWorld()->SpawnActor<AGameManager>(SpawnedGameManager);

}
