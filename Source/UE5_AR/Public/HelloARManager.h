// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CustomGameMode.h"
#include "GameFramework/Actor.h"
#include "HelloARManager.generated.h"

class UARSessionConfig;
class AARPlaneActor;
class UARPlaneGeometry;
class ACustomGameState;

UCLASS()
class UE5_AR_API AHelloARManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHelloARManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Create a default Scene Component
	UPROPERTY(Category = "SceneComp", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(Category = "LowestPlane", EditAnywhere, BlueprintReadWrite)
	AARPlaneActor* LowestPlaneActor;		//Reference to the lowest PlaneActor.
	AARPlaneActor* FirstTable;		//Reference to the first table detected.

	UFUNCTION(BlueprintCallable)
	void DestroyAllPlanes();
protected:
	


	AARPlaneActor* SpawnPlaneActor();	//Function to spawn Plane Actor.	
	FColor GetPlaneColor(int Index);		

	void UpdatePlaneActors();		// Updates the plane actors on every frame as long as the AR Session is running.
	void ResetARCoreSession();		//Reset AR session.
	void PlaneTagUpdate();		//Function to update planes tags.
	void AssignTag(AARPlaneActor* CurrentPlane);		//Function to decide what tag is assigned to iterated plane.

	//Base plane actor for geometry detection
	AARPlaneActor* PlaneActor;		//SinglePlaneActor


	ACustomGameMode* GM;		//Reference to CustomGameMode.
	ACustomGameState* GS;		//Reference to CustomGameState.

	TMap<UARPlaneGeometry*, AARPlaneActor*> PlaneActors;			//Map of geometry planes.

	//Index for plane colours adn array of colours.
	int PlaneIndex = 0;
	TArray<FColor> PlaneColors;
	
	float TableHeight;	//Height at which planes are identified as table.
	float WallSize;		//Size at which planes are identified as wall.
	bool IsFirstTableDetected = false;
public:
	// Configuration file for AR Session.
	UARSessionConfig* Config;

};
