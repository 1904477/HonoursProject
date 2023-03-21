// Fill out your copyright notice in the Description page of Project Settings.


#include "GunPickup.h"

AGunPickup::AGunPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SetRootComponent(SkeletalMesh);

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("GunSceneComponent"));
	SceneComponent->SetupAttachment(SkeletalMesh);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(SkeletalMesh);

	BoxComponent->SetWorldScale3D(FVector(0.4, 1, 0.5));
	BoxComponent->SetRelativeLocation(FVector(-40, 0, 30));
	BoxComponent->SetCollisionProfileName("BlockAll");

}

void AGunPickup::BeginPlay()
{
	Super::BeginPlay();
	auto Temp1 = GetWorld()->GetGameState();
	GS = Cast<ACustomGameState>(Temp1);

	FVector origin;
	FVector boxExtent;
	GetActorBounds(false, origin, boxExtent);
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y,GetActorLocation().Z+ BoxComponent->GetScaledBoxExtent().Z));
}

void AGunPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
