// Fill out your copyright notice in the Description page of Project Settings.


#include "StepObstacle.h"


AStepObstacle::AStepObstacle()
{
	Tags.Add("Step");
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("	StaticMesh'/Game/StarterContent/Props/SM_Stairs.SM_Stairs'"));
	StaticMeshComponent->SetStaticMesh(MeshObj.Object);


}

// Called when the game starts or when spawned
void AStepObstacle::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStepObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

