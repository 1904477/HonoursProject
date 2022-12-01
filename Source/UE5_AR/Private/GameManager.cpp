// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

#include <string>

#include "CustomActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "HelloARManager.h"
#include "ARPin.h"
#include "ARBlueprintLibrary.h"
#include "HelloARManager.h"
#include <string>

#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AGameManager::AGameManager() :
	Player(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	playerController = UGameplayStatics::GetPlayerController(this, 0);
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameManager::SpawnCube()
{
	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	const FVector MyLoc(-300, 0, 0);
	ACustomActor* CustomActor = GetWorld()->SpawnActor<ACustomActor>(MyLoc, MyRot, SpawnInfo);

}

void AGameManager::SpawnPlayer()
{
		const FActorSpawnParameters SpawnInfo;
		const FRotator MyRot(0, 0, 0);
		const FVector MyLoc(0, 0, 1000);
		Player = GetWorld()->SpawnActor<APlaceablePlayer>(PlacableToSpawn, MyLoc, MyRot, SpawnInfo);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("ACTOR SPAWNED"));
}

FTransform AGameManager::LineTraceResult(FVector ScreenPos)		//Function to return touch position in real world
{
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
bool AGameManager::WorldHitTest(FVector ScreenPos,FHitResult& HitResult)
{
	bool deprojectionSuccess = UGameplayStatics::DeprojectScreenToWorld(playerController, FVector2D(ScreenPos), /*out*/
		WorldPos, /*out*/ WorldDir);
	// return if the operation was successful
	if (deprojectionSuccess) {
		// construct trace vector (from point clicked to 1000.0 units beyond in same direction)
		FVector traceEndVector = WorldDir * 1000.0;
		traceEndVector = WorldPos + traceEndVector;
		// perform line trace (Raycast)
		bool traceSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, WorldPos, traceEndVector, ECollisionChannel::ECC_WorldDynamic);
		// return if the operation was successful
		return traceSuccess;
	}
	else
		return false;

}

void AGameManager::LineTraceSpawnActor(FVector ScreenPos)	
{
		
		TraceResultTransform = LineTraceResult(ScreenPos);		//Get the location of touch in the real world to spawn Player
		//Spawn a new Actor at the location if not done yet
		if (!Player)		//If the player has not been spawned
		{
			if(!TraceResultTransform.Equals(FTransform::Identity))
			{
				SpawnPlayer();
				//Player->SetActorTransform(TraceResultTransform);	//Set transform to pinTransform
				Player->SetActorLocation(FVector(
						TraceResultTransform.GetLocation().X,
						TraceResultTransform.GetLocation().Y
					,TraceResultTransform.GetLocation().Z+100));	//Add offset for collision with plane, else the player will fall under the planes
			}
		}
		if (WorldHitTest(ScreenPos, HR))
		{
			AARPlaneActor* planeActorRef = Cast<AARPlaneActor>(HR.GetActor());
			if (planeActorRef)
			{
				Player->LastClickedPlane = planeActorRef;
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Plane clicked"));
			}
		}
}
void AGameManager::LineTraceMovement(FVector ScreenPos)		//Movement function for player
{

	Player->IsPlayerIdle = false;		//When player moves, it is not idle anymore
	TraceResultTransform = LineTraceResult(ScreenPos);		//Get location of touch
	MoveLocation =TraceResultTransform.GetLocation();		//Set player move location to touch location
	if (WorldHitTest(ScreenPos, HR))
	{
		AARPlaneActor* planeActorRef = Cast<AARPlaneActor>(HR.GetActor());
		if (planeActorRef)
		{
			if (Player->LastClickedPlane)
			{
				if ((planeActorRef->GetActorLocation() - Player->LastClickedPlane->GetActorLocation()).Length() > 200)
				{
					Player->CanPlayerJump = true;
				}
				Player->LastClickedPlane = planeActorRef;
			}
		}
	}
	if(!TraceResultTransform.Equals(FTransform::Identity))
	{
		//get distance of height between the actual location and the move location to detect whether player jumps or run towards location
		float DistanceToLocationZ = MoveLocation.Z - Player->GetActorLocation().Z;
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, std::to_string(DistanceToLocationZ).c_str());
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, std::to_string(MoveLocation.Z).c_str());
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, std::to_string(Player->GetActorLocation().Z).c_str());
		if (abs(DistanceToLocationZ) <= 25.f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("walk"));
			Player->DoesPlayerWalk = true;
			Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
		else if(abs(DistanceToLocationZ) > 25.f || Player->CanPlayerJump == true)
		{
			Player->DoesPlayerJump = true;
			Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			Player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("jump"));
		}
	}
}

void AGameManager::flipPlaneVisibility()	//Function to flip the plane visibility, called in the widget blueprints.
{
	auto Temp = GetWorld()->GetGameState();
	ACustomGameState * GameState = Cast<ACustomGameState>(Temp);
	GameState->SetAreARPlanesDisplayed(!GameState->GetAreARPlanesDisplayed());
}




//else		IOS
//{
//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ARPin is invalid"));
//	//Spawn a new Actor at the location if not done yet
//	if (!SpawnedActor)
//	{
//		const FActorSpawnParameters SpawnInfo;
//		const FRotator MyRot(0, 0, 0);
//		const FVector MyLoc(0, 0, 0);
//		SpawnedActor = GetWorld()->SpawnActor<APlaceableActor>(PlacableToSpawn, MyLoc, MyRot, SpawnInfo);
//	}
//	SpawnedActor->SetActorTransform(TrackedTF);
//	SpawnedActor->SetActorScale3D(FVector(0.2, 0.2, 0.2));
//}
//		}
//	}
//}

//if (!GetPlaceableSelected() && SpawnedActor)
//{
//	FHitResult HitResult;
//	//if distance from touch to actor is far enough from actor, reset actor position, if distance from touch to actor is close, don't transform and change colour
//	// Perform a hitTest, get the return values as hitTesTResult
//	WorldHitTest(FVector2D(ScreenPos), HitResult);
//	// Get object of actor hit.
//	UClass* hitActorClass = UGameplayStatics::GetObjectClass(HitResult.GetActor());
//	// if we've hit a target.
//	if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, APlaceableActor::StaticClass()))
//		SetPlaceableSelected(true);
//}
/*	if (GetPlaceableSelected() == true)
	{*/
//}

