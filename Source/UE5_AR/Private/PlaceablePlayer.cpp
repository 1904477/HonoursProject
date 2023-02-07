// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceablePlayer.h"
#include "GameManager.h"
#include "CustomGameMode.h"
#include "NavigationSystem.h"
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
	enemyStatus = Idle;
	wanderRadius = 1500.0f;
	if(!AIController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, TEXT("AIController not found"));
	}

}

void APlaceablePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovePlayer();
	enemyStatusTimer += GetWorld()->GetDeltaSeconds();
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
	UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIController);


	if (!NavigationArea)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("bad pointer"));

		return;
	}


	if (AIController)
	{
		if (enemyStatusTimer > 4.0f)		//Enemies change state every x seconds (unless suspicious, then timer is longer)
		{
			int randomChoice = FMath::RandRange(1, 2);		//Random choice
			FNavLocation endPosi = FNavLocation(GetActorLocation());
			switch (randomChoice)
			{
			case 1: enemyStatus = Idle;
				break;
			case 2:
				enemyStatus = Wandering;
				
				if (NavigationArea->GetRandomPointInNavigableRadius(GetActorLocation(), wanderRadius, endPosi)) {		//Get random position in navmesh
					// if we were successfull in finding a new location...
					FVector dest = endPosi.Location;		//Save random position in navmesh in FVector
					AIController->MoveToLocation(dest, -1, false, true);		//Move to destination
				}
				break;
			//case 3: enemyStatus = Suspicious;
			//	moveTo = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
			//	moveTo = moveTo - GetActorLocation();
			//	AIController->MoveToLocation(moveTo, -1, false, true);
			//	break;
			}
			enemyStatusTimer = 0;
		}
	}
}