// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameMode.h"
#include "GameManager.h"
#include "CustomARPawn.h"
#include "CustomGameState.h"
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
	//GetWorldTimerManager().SetTimer(Ticker, this, &ACustomGameMode::SpawnCube, 4.0f, true, 0.0f);
}



void ACustomGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

bool ACustomGameMode::WorldHitTest(FVector2D screenTouchPos, FHitResult& HitResult)
{
	// Get player controller
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	// Perform deprojection taking 2d clicked area and generating reference in 3d world-space.
	FVector worldPosition;
	FVector worldDirection; // Unit Vector
	bool deprojectionSuccess = UGameplayStatics::DeprojectScreenToWorld(playerController, screenTouchPos, /*out*/
		worldPosition, /*out*/ worldDirection);
	// construct trace vector (from point clicked to 1000.0 units beyond in same direction)
	FVector traceEndVector = worldDirection * 1000.0;
	traceEndVector = worldPosition + traceEndVector;
	// perform line trace (Raycast)
	bool traceSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, worldPosition, traceEndVector,ECollisionChannel::ECC_WorldDynamic);
	// return if the operation was successful
	return traceSuccess;
}


void ACustomGameMode::SpawnInitialActors()
{
	// Spawn an instance of the HelloARManager class
	UARSessionConfig* Config = NewObject<UARSessionConfig>();
	AHelloARManager* Manager = GetWorld()->SpawnActor<AHelloARManager>();
	GameManager = GetWorld()->SpawnActor<AGameManager>(SpawnedGameManager);

	UARBlueprintLibrary::StartARSession(Config);
}
//Calculate the distance from touch input to actor, could be useful depending on the game.
						//FVector DistTouchToActor = TraceResult[0].GetLocalToWorldTransform().GetLocation() - SpawnedActor->GetActorLocation();
						//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Dist Touch to actor: %f"), DistTouchToActor.Size()));
						//if(DistTouchToActor.Size()>50)
						//{
						//	SpawnedActor->SetActorTransform(PinTF);
						//	SpawnedActor->PinComponent = ActorPin;
						//}