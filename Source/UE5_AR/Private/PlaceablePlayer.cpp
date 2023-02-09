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
	SetActorScale3D(FVector(1.3, 1.3, 1.3));
	enemyStatus = Idle;
	wanderRadius = 1500.0f;

	NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIController);
}

void APlaceablePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EnemyStatusManager();
	enemyStatusTimer += GetWorld()->GetDeltaSeconds();
}

// Called to bind functionality to input
void APlaceablePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlaceablePlayer::EnemySuspicious()
{
			//case 3: enemyStatus = Suspicious;
			//	moveTo = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
			//	moveTo = moveTo - GetActorLocation();
			//	AIController->MoveToLocation(moveTo, -1, false, true);
			//	break;
}

void APlaceablePlayer::EnemyWander()
{
	FNavLocation endPosi = FNavLocation(GetActorLocation());

	if (!NavigationArea)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("bad pointer"));
	if (NavigationArea->GetRandomPointInNavigableRadius(GetActorLocation(), wanderRadius, endPosi)) //Get random position in navmesh
	{		
		// if we were successfull in finding a new location...
		FVector dest = endPosi.Location;		//Save random position in navmesh in FVector
		AIController->MoveToLocation(dest, -1, false, true);		//Move to destination
	}
}
void APlaceablePlayer::EnemyStatusManager()
{
	if (AIController)
	{
		int randomChoice = FMath::RandRange(1, 2);		//Random choice
		if (enemyStatusTimer > 1.3f && enemyStatus != Suspicious)		//Enemies change state every x seconds (unless suspicious, then timer is longer)
		{
			switch (randomChoice)
			{
			case 1: enemyStatus = Idle;
				break;
			case 2:
				enemyStatus = Wandering;
				EnemyWander();
				break;
			}
			enemyStatusTimer = 0;
		}
	}
}