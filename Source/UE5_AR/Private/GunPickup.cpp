// Fill out your copyright notice in the Description page of Project Settings.


#include "GunPickup.h"

AGunPickup::AGunPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsPicked = false;

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Statue.SM_Statue'"));
	StaticMeshComponent->SetStaticMesh(MeshObj.Object);
}

void AGunPickup::BeginPlay()
{
	Super::BeginPlay();
}

void AGunPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
