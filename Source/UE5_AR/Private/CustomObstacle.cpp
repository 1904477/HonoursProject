// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomObstacle.h"

// Sets default values
ACustomObstacle::ACustomObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SceneComponent);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ACustomObstacle::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACustomObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

