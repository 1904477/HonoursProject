// Fill out your copyright notice in the Description page of Project Settings.


#include "GraveObject.h"
#include "HelloARManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ARPlaneActor.h"

AGraveObject::AGraveObject()
{
	Tags.Add("Step");
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("	StaticMesh'/Game/Assets/Sepultura_Sepultura.Sepultura_Sepultura'"));
	StaticMeshComponent->SetStaticMesh(MeshObj.Object);

	StaticMeshComponent->SetWorldScale3D(FVector(0.3, 0.3, 0.3));
}

void AGraveObject::BeginPlay()
{
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

void AGraveObject::Tick(float DeltaTime)
{
}
