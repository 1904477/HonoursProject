// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomObject.h"

// Sets default values
ACustomObject::ACustomObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Set SceneComponent as root.
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	//Static mesh is attached to SceneComponent.
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SceneComponent);
	
	//Set BoxCollision and attach it to Scenecomponent.
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ACustomObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACustomObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

