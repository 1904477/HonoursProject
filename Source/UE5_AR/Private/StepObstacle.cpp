// Fill out your copyright notice in the Description page of Project Settings.


#include "StepObstacle.h"
#include "HelloARManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ARPlaneActor.h"

AStepObstacle::AStepObstacle()
{
	Tags.Add("Step");
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("	StaticMesh'/Game/StarterContent/Props/SM_Stairs.SM_Stairs'"));
	StaticMeshComponent->SetStaticMesh(MeshObj.Object);
	SetActorScale3D(FVector(0.4, 0.7,0.4));

	StaticMeshComponent->SetStaticMesh(MeshObj.Object);
	BoxComponent->SetWorldScale3D(FVector(1.2f, 3.0f, 0.7f));
	BoxComponent->SetRelativeLocation(FVector(-40, 0, 30));
	BoxComponent->SetCollisionProfileName("BlockAll");
}

// Called when the game starts or when spawned
void AStepObstacle::BeginPlay()
{
	Super::BeginPlay();
	auto GM = GetWorld()->GetAuthGameMode();
	CustomGameMode = Cast<ACustomGameMode>(GM);

	if (UGameplayStatics::GetPlatformName() == "IOS" || UGameplayStatics::GetPlatformName() == "Android")
	{
		FVector origin;
		FVector boxExtent;
		GetActorBounds(false, origin, boxExtent);
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, CustomGameMode->ARManager->LowestPlaneActor->GetActorLocation().Z + BoxComponent->GetScaledBoxExtent().Z));

	}
}

// Called every frame
void AStepObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

