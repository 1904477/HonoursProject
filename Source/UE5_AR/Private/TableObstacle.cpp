// Fill out your copyright notice in the Description page of Project Settings.


#include "TableObstacle.h"


ATableObstacle::ATableObstacle()
{
	Tags.Add("Table");
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Shelf.SM_Shelf'"));
	StaticMeshComponent->SetStaticMesh(MeshObj.Object);
}

// Called when the game starts or when spawned
void ATableObstacle::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATableObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

