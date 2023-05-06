// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ARPin.h"
#include "GameObjectsSpawner.h"
#include "ARBlueprintLibrary.h"
#include "HelloARManager.h"
// Sets default values
AGameManager::AGameManager() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	//Get GameState.
	auto GS = GetWorld()->GetGameState();
	GameState = Cast<ACustomGameState>(GS);
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If environment is scanned but game is not started, spawn GameObjectsSpawner.
	if (GameState->GetIsEnvironmentScanned() == true&& hasFinishedScanning == false && GameState->GetHasGameStarted()==false)
	{
		SpawnGameObjectsSpawner(); 
		hasFinishedScanning = true;		
	}
}

void AGameManager::SpawnGameObjectsSpawner()
{
	//Function to spawn GameObjectsSpawner.
	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	const FVector MyLoc(0, 0, 0);
	ObjectsSpawner = GetWorld()->SpawnActor<AGameObjectsSpawner>(GameObjectsSpawnerToSpawn,MyLoc, MyRot, SpawnInfo);
}

FTransform AGameManager::LineTraceResult(FVector ScreenPos)		//Function to return touch position in real world.
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector WorldPos;
	FVector WorldDir;
	
	//Deproject screen touch to real world, cast the screenPosition vector passed from CustomARPawn to a 2D vector for the screen coordinates.
	UGameplayStatics::DeprojectScreenToWorld(playerController, FVector2D(ScreenPos), WorldPos, WorldDir); 
	auto TraceResult = UARBlueprintLibrary::LineTraceTrackedObjects(FVector2D(ScreenPos), false, false, false, true);
	//Checks if the location is valid
	if (TraceResult.IsValidIndex(0))
	{
		// Get the first found object in the line trace - ignoring the rest of the array elements
		auto TrackedTF = TraceResult[0].GetLocalToWorldTransform();
		if (FVector::DotProduct(TrackedTF.GetRotation().GetUpVector(), WorldDir) < 0)		//If the position is not under the plane, dot product for angle
		{
			return TrackedTF;		//Return deprojected location of touch (will be the location in the real world.
		}
	}
	return FTransform::Identity;
}


void AGameManager::flipPlaneVisibility()	//Function to flip the plane visibility, called in the widget blueprints.
{
	auto Temp = GetWorld()->GetGameState();
	GameState->SetAreARPlanesDisplayed(!GameState->GetAreARPlanesDisplayed());
}
