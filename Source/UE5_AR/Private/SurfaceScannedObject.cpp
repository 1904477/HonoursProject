// Fill out your copyright notice in the Description page of Project Settings.


#include "SurfaceScannedObject.h"

// Sets default values
ASurfaceScannedObject::ASurfaceScannedObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Shelf.SM_Shelf'"));
	SetActorScale3D(FVector(2, 1, 2));
	StaticMeshComponent->SetStaticMesh(MeshObj.Object);
	BoxComponent->SetWorldScale3D(FVector(1.0f, 3.2f, 0.4f));
	BoxComponent->SetRelativeLocation(FVector(0, 0, -10));
	BoxComponent->SetCollisionProfileName("BlockAll");

}

// Called when the game starts or when spawned
void ASurfaceScannedObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASurfaceScannedObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

