// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceablePlayer.h"
#include "GameManager.h"
#include "CustomGameMode.h"
APlaceablePlayer::APlaceablePlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	
}
void APlaceablePlayer::BeginPlay()
{
	Super::BeginPlay();
	auto Temp = GetWorld()->GetAuthGameMode();
	GM = Cast<ACustomGameMode>(Temp);
	GameManager = GM->GameManager;
	GetMesh()->BodyInstance.bLockYRotation = true;
	GetMesh()->BodyInstance.bLockXRotation = true;
	AIController = Cast<AAIController>(GetController());
	if(!AIController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, TEXT("AIController not found"));
	}
	
}

void APlaceablePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsPlayerIdle==false)
	{
		MovePlayer();
	}
	
}

// Called to bind functionality to input
void APlaceablePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind various player inputs to functions
	// There are a few types - BindTouch, BindAxis, and BindEvent.  
}

void APlaceablePlayer::MovePlayer()
{
	if(AIController)
	{
		FVector ActualPosition = GetActorLocation();
		//FVector MoveDirection =  - ActualPosition;
		//FRotator NewRot = (GameManager->MoveLocation - ActualPosition).Rotation();
		//SetActorRotation(NewRot);

			//AIController->MoveTo(GameManager->MoveLocation );
		AIController->MoveToLocation(GameManager->MoveLocation,-1,false, true );


	}
}
