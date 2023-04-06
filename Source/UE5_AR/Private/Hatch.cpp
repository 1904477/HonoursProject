// Fill out your copyright notice in the Description page of Project Settings.
#include "Hatch.h"

// Sets default values
AHatch::AHatch()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrame->SetupAttachment(SceneComponent);

	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj2(TEXT("StaticMesh'/Game/StarterContent/Props/SM_DoorFrame.SM_DoorFrame'"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Props/SM_Door.SM_Door'"));

	StaticMeshComponent->SetStaticMesh(MeshObj.Object);
	DoorFrame->SetStaticMesh(MeshObj2.Object);

	Tags.Add("Hatch");
}

// Called when the game starts or when spawned
void AHatch::BeginPlay()
{
	Super::BeginPlay();

	auto Temp1 = GetWorld()->GetGameState();
	GS = Cast<ACustomGameState>(Temp1);
	auto P = GetWorld()->GetFirstPlayerController()->GetPawn();
	Player = Cast<ACustomARPawn>(P);

}

// Called every frame
void AHatch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimerManager();

	if (GS->GetIsHatchOpen())
	{
		float rotSpeed = 10;
		StaticMeshComponent->SetRelativeRotation(FRotator(0,0,0));
		if ((Player->camLocation - GetActorLocation()).Length() < 100)
		{
			GS->SetDidPlayerWin(true);

		}
	}
}

void AHatch::TimerManager()
{
	if (GS->GetHatchOpenTimerMin() > 0 || GS->GetHatchOpenTimerSec() > 0)
	{
		if (GS->GetHatchOpenTimerSec() <= 0)
		{
			GS->SetHatchOpenTimer(GS->GetHatchOpenTimerMin() - 1, 60);
		}
		GS->SetHatchOpenTimer(GS->GetHatchOpenTimerMin(), GS->GetHatchOpenTimerSec() - GetWorld()->GetDeltaSeconds());
	}
	else
	{
		GS->SetIsHatchOpen(true);
	}
}

