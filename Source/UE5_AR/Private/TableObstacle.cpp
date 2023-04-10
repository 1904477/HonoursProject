// Fill out your copyright notice in the Description page of Project Settings.


#include "TableObstacle.h"
#include "HelloARManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ARPlaneActor.h"

ATableObstacle::ATableObstacle()
{
	Tags.Add("Table");
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Shelf.SM_Shelf'"));
	SetActorScale3D(FVector(2, 1, 2));
	StaticMeshComponent->SetStaticMesh(MeshObj.Object);
	BoxComponent->SetWorldScale3D(FVector(1.0f, 3.2f, 0.4f));
	BoxComponent->SetRelativeLocation(FVector(0, 0, -10));
	BoxComponent->SetCollisionProfileName("BlockAll");
	
	//SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, CustomGameMode->ARManager->LowestPlaneActor->GetActorLocation().Z+ BoxComponent->GetScaledBoxExtent().Z));
}

// Called when the game starts or when spawned
void ATableObstacle::BeginPlay()
{
	auto GM = GetWorld()->GetAuthGameMode();
	CustomGameMode = Cast<ACustomGameMode>(GM);

	if (UGameplayStatics::GetPlatformName() == "IOS" || UGameplayStatics::GetPlatformName() == "Android")
	{
		FVector origin;
		FVector boxExtent;
		GetActorBounds(false, origin, boxExtent);
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, CustomGameMode->ARManager->LowestPlaneActor->GetActorLocation().Z + (BoxComponent->GetScaledBoxExtent().Z*2)));
	}
	else
	{
		FVector origin;
		FVector boxExtent;
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, BoxComponent->GetScaledBoxExtent().Z * 2));
	}
	Super::BeginPlay();
}

// Called every frame
void ATableObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

