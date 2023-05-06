// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloARManager.h"
#include "CustomGameMode.h"
#include "CustomGameState.h"
#include "ARPlaneActor.h"
#include "ARPin.h"
#include "ARSessionConfig.h"
#include "ARBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <limits>

// Sets default values
AHelloARManager::AHelloARManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// This constructor helper is useful for a quick reference within UnrealEngine and if you're working alone. But if you're working in a team, this could be messy.
	// If the artist chooses to change the location of an art asset, this will throw errors and break the game.
	// Instead let unreal deal with this. Usually avoid this method of referencing.
	// For long term games, create a Data-Only blueprint (A blueprint without any script in it) and set references to the object using the blueprint editor.
	// This way, unreal will notify your artist if the asset is being used and what can be used to replace it.
	static ConstructorHelpers::FObjectFinder<UARSessionConfig> ConfigAsset(TEXT("ARSessionConfig'/Game/Blueprints/HelloARSessionConfig.HelloARSessionConfig'"));
	Config = ConfigAsset.Object;
	Config->bUseSceneDepthForOcclusion = true;
	Config->SetSessionTrackingFeatureToEnable(EARSessionTrackingFeature::SceneDepth);
	//Populate the plane colours array
	TableHeight = 50;
	WallSize = 10;
}

// Called when the game starts or when spawned
void AHelloARManager::BeginPlay()
{
	Super::BeginPlay();

	//Get Gamemode.
	auto Temp = GetWorld()->GetAuthGameMode();		
	GM = Cast<ACustomGameMode>(Temp);
	//Get Gamestate.
	auto Temp2 = GetWorld()->GetGameState();	
	GS = Cast<ACustomGameState>(Temp2);
}

// Called every frame
void AHelloARManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (UARBlueprintLibrary::GetARSessionStatus().Status)
	{
	case EARSessionStatus::Running:
			UpdatePlaneActors();		//Update all plane actors.
			//Update plane tags as long as the user has not confirmed that the environment has been scanned.
			if (GS->GetIsEnvironmentScanned() == false)
			{
				PlaneTagUpdate();
			}
		break;
	case EARSessionStatus::FatalError:		//In case of fatal error in the AR sessions
		ResetARCoreSession();		//Reset the session
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("SESSION RESET, FATAL ERROR"));
		UARBlueprintLibrary::StartARSession(Config);	
		break;
	}
}


//Updates the geometry actors in the world
void AHelloARManager::UpdatePlaneActors()
{
	//Get all world geometries and store in an array.
	auto Geometries = UARBlueprintLibrary::GetAllGeometriesByClass<UARPlaneGeometry>();
	//Loop through all geometries.
	for (auto& It : Geometries)
	{
			//Check if current plane exists.
			if (PlaneActors.Contains(It))
			{
				AARPlaneActor* CurrentPActor = *PlaneActors.Find(It);
				//Check if plane is subsumed.
				if (It->GetSubsumedBy()->IsValidLowLevel())
				{
					CurrentPActor->Destroy();
					PlaneActors.Remove(It);
					break;
				}
				else
					CurrentPActor->UpdatePlanePolygonMesh();
			}
			else
			{
				//Get tracking state switch.
				switch (It->GetTrackingState())
				{
				case EARTrackingState::Tracking:
					if (!It->GetSubsumedBy()->IsValidLowLevel())
					{
						//Spawn and update poligon meshes.
						PlaneActor = SpawnPlaneActor();
						PlaneActor->ARCorePlaneObject = It;

						PlaneActors.Add(It, PlaneActor);
						PlaneActor->UpdatePlanePolygonMesh();
						PlaneIndex++;

						if (!LowestPlaneActor)		//If lowest plane reference is not set, set it to first plane.
						{
							LowestPlaneActor = PlaneActor;
							PlaneActor->Tags.Add("floor");
						}

						//If newly detected plane is lower than the LowestPlaneActor, update LowestPlaneActor to be the newly detected one.
						else if (LowestPlaneActor->GetActorLocation().Z > PlaneActor->GetActorLocation().Z)
						{
							LowestPlaneActor = PlaneActor;
							PlaneActor->Tags.Add("floor");
						}
							PlaneActor->Tags.Add("uncathegorised");

					}
					break;
				}
			}
	}
}

//Simple spawn function for the tracked AR planes.
AARPlaneActor* AHelloARManager::SpawnPlaneActor()
{
	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	const FVector MyLoc(0, 0, 0);
	AARPlaneActor* CustomPlane = GetWorld()->SpawnActor<AARPlaneActor>(MyLoc, MyRot, SpawnInfo);
	return CustomPlane;
}

void AHelloARManager::ResetARCoreSession()
{
	//Get all actors in the level and destroy them as well as emptying the respective arrays
	TArray<AActor*> Planes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AARPlaneActor::StaticClass(), Planes);
	for ( auto& It : Planes)
		It->Destroy();
	
	Planes.Empty();
	PlaneActors.Empty();
}

void AHelloARManager::PlaneTagUpdate()
{
	//Get all world geometries and store in an array.
	auto Geometries = UARBlueprintLibrary::GetAllGeometriesByClass<UARPlaneGeometry>();
	//Loop through all geometries.
	for (auto& It : Geometries)
	{
		//Check if current plane exists.
		if (PlaneActors.Contains(It))
		{
			AARPlaneActor* CurrentPActor = *PlaneActors.Find(It);

			AssignTag(CurrentPActor);
		}
	}
}

//Function to update tags.
void AHelloARManager::AssignTag(AARPlaneActor* CurrentPActor)
{
	FVector origin;
	FVector boxExtent;
	CurrentPActor->GetActorBounds(false, origin, boxExtent);

	//Classify surfaces based on different attributes of surfaces. If the plane has a tag, but a more appropriate tag should be set as the detection is dynamic, the tags of the actor are
	//emptied, the appropriate tag is set and the number of the respective surfaces is updated.
	if(CurrentPActor->Tags[0]!="table")
	{
		if ((CurrentPActor->GetActorLocation().Z >= LowestPlaneActor->GetActorLocation().Z + TableHeight) && boxExtent.Z<WallSize && CurrentPActor != LowestPlaneActor)
		{
			if (!CurrentPActor->Tags.IsEmpty())
				if (CurrentPActor->Tags[0] != "table")
				{
					if(CurrentPActor->ActorHasTag("step"))
						gymSteps--;
					CurrentPActor->Tags.Empty();
				}

			if (IsFirstTableDetected == false)
			{
				FirstTable = CurrentPActor;
				IsFirstTableDetected = true;
			}
			CurrentPActor->Tags.Add("table");
			tables++;
		}
	}
	if(CurrentPActor->Tags[0]!="step")
	{
		if ((CurrentPActor->GetActorLocation().Z < LowestPlaneActor->GetActorLocation().Z + TableHeight) && CurrentPActor != LowestPlaneActor&& boxExtent.Z<2)
		{
			if (!CurrentPActor->Tags.IsEmpty())
				if (CurrentPActor->Tags[0] != "step")
				{
					if(CurrentPActor->ActorHasTag("table"))
						tables--;
					CurrentPActor->Tags.Empty();
				}
			CurrentPActor->Tags.Add("step");
			gymSteps++;

		}
	}
	if(CurrentPActor->Tags[0]!="wall")
	{
		if (boxExtent.Z>= WallSize && CurrentPActor != LowestPlaneActor)
		{
			if (!CurrentPActor->Tags.IsEmpty())
				if (CurrentPActor->Tags[0] != "wall")
				{
					if(CurrentPActor->ActorHasTag("table"))
						tables--;
					if(CurrentPActor->ActorHasTag("step"))
						gymSteps--;
					CurrentPActor->Tags.Empty();
				}
			CurrentPActor->Tags.Add("wall");
		}
	}
	if (LowestPlaneActor->GetActorLocation().Z > CurrentPActor->GetActorLocation().Z)
	{
		if (!CurrentPActor->Tags.IsEmpty())
		{
			if (CurrentPActor->Tags[0] != "floor")
			{
				if(CurrentPActor->ActorHasTag("table"))
					tables--;
				if(CurrentPActor->ActorHasTag("step"))
					gymSteps--;
				CurrentPActor->Tags.Empty();
				CurrentPActor->Tags.Add("floor");
			}
		}
		else
		{
			CurrentPActor->Tags.Add("floor");
		}
		LowestPlaneActor = CurrentPActor;
	}
}


void AHelloARManager::DestroyAllPlanes()
{
	auto Geometries = UARBlueprintLibrary::GetAllGeometriesByClass<UARPlaneGeometry>();
	auto planes = UARBlueprintLibrary::GetAllGeometriesByClass<AARPlaneActor>();

	while (planes.Num() > 0) {
		planes[0]->Destroy();
		planes.RemoveAt(0);
	}

	while (Geometries.Num() > 0) {
		Geometries[0]->ConditionalBeginDestroy();
		Geometries.RemoveAt(0);
	}
	ResetARCoreSession();

}