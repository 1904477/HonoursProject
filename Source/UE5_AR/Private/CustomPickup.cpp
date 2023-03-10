// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPickup.h"

// Sets default values
ACustomPickup::ACustomPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SceneComponent);

}

// Called when the game starts or when spawned
void ACustomPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

