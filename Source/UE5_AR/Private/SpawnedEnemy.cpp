// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnedEnemy.h"
#include "GameManager.h"
#include "CustomGameMode.h"
#include "HelloARManager.h"
#include "NavigationSystem.h"
#include "ARPlaneActor.h"
#include "CustomARPawn.h"
#include "ARBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

ASpawnedEnemy::ASpawnedEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}
void ASpawnedEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//SetActorScale3D(FVector(0.6, 0.6, 0.6));			//Scale Enemy

	auto PTemp = GetWorld()->GetFirstPlayerController()->GetPawn();		
	Player = Cast<ACustomARPawn>(PTemp);

	GetMesh()->BodyInstance.bLockYRotation = true;		//Lock rotation so that characters do not rotate in other directions
	GetMesh()->BodyInstance.bLockXRotation = true;

	EnemyStatus = Spawning;		//Starting status is idle.
	WanderRadius = 500.0f;		//Radius for wander area.
	StateSwitchTimer = 2.5f;	//How long before the enemy switches state.

	AIController = Cast<AAIController>(GetController());
	NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIController);
}

void ASpawnedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EnemyStatusManager();
	//DrawDebugString(GetWorld(), FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 10), FString::SanitizeFloat(Health), NULL, FColor::Cyan, 0.01, false, 3);

	if (Health <= 0)
	{
		Destroy();
		int tmp = GS->GetScore();
		GS->SetScore(tmp += 10);
	}
	FallDetection();
	switch (EnemyStatus)
	{
	case 0: 
		GetCharacterMovement()->MaxWalkSpeed = 0.1; // replace 300 with your desired speed()
		break;
	case 1:
		GetCharacterMovement()->MaxWalkSpeed = 40.0f; // replace 300 with your desired speed()
		break;
	case 2:
		GetCharacterMovement()->MaxWalkSpeed = 50.0f; // replace 300 with your desired speed()
		break;
	case 3:
		GetCharacterMovement()->MaxWalkSpeed = 70.0f; // replace 300 with your desired speed()
		break;
	case 4:
		GetCharacterMovement()->MaxWalkSpeed = 0.1; // replace 300 with your desired speed()
		break;
	}
}

// Called to bind functionality to input
void ASpawnedEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASpawnedEnemy::EnemySuspicious()
{
	SuspiciousTimer += GetWorld()->GetDeltaSeconds();	//Keep track of how long enemy has been suspicious.

	if (SuspiciousTimer > 5.0f && (Player->camLocation - GetActorLocation()).Length() > GM->GameManager->EnemySuspiciousDistance) {		//Enemy is suspicious for 4 seconds
		SuspiciousTimer = 0;			//Reset timer.
		EnemyStatus = Idle;		//After the enemy is suspicious, it becomes idle
	}
	
	AIController->MoveToLocation(Player->camLocation, -1,true,true);
}

void ASpawnedEnemy::EnemyWander()
{
	FNavLocation endPosi = FNavLocation(GetActorLocation());
	GetCharacterMovement()->MaxWalkSpeed = 40.0f; // replace 300 with your desired speed()

	if (!NavigationArea)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("bad pointer"));
	if (NavigationArea->GetRandomPointInNavigableRadius(GetActorLocation(), WanderRadius, endPosi)) //Get random position in navmesh
	{		
		// if we were successfull in finding a new location...
		FVector dest = endPosi.Location;		//Save random position in navmesh in FVector
		AIController->MoveToLocation(dest, -1, false, true);		//Move to destination
	}

}

void ASpawnedEnemy::EnemyCharging()
{
	GetCharacterMovement()->MaxWalkSpeed = 90.0f;		//In attack state, enemy is faster
	AIController->MoveToLocation(Player->camLocation, -1, true, true);
}

void ASpawnedEnemy::ClosestObstacleChecker()
{

}

void ASpawnedEnemy::FallDetection()
{
	if (UGameplayStatics::GetPlatformName() == "IOS" || UGameplayStatics::GetPlatformName() == "Android")
	{
		if (abs(GetActorLocation().Z - GM->ARManager->LowestPlaneActor->GetActorLocation().Z) > 20)
		{
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GM->ARManager->LowestPlaneActor->GetActorLocation().Z + GetCapsuleComponent()->GetLocalBounds().BoxExtent.Z));
		}
	}
	else
	{
		if (GetActorLocation().Z < 100)
		{
			SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 0 + GetCapsuleComponent()->GetLocalBounds().BoxExtent.Z));
		}
	}
}

void ASpawnedEnemy::EnemyStatusManager()
{
	if (AIController)
	{
		if (EnemyStatus != Spawning)
		{
			if (EnemyStatus != Suspicious && EnemyStatus != Charging && EnemyStatus != Attacking)
			{
				EnemyStatusTimer += GetWorld()->GetDeltaSeconds();
				int randomChoice = FMath::RandRange(1, 2);		//Random choice in Enemy Finite State Machine
				if (EnemyStatusTimer > StateSwitchTimer)		//Enemies change state every x seconds (unless suspicious, then timer is longer)
				{
					switch (randomChoice)
					{
					case 1: EnemyStatus = Idle;
=						break;
					case 2:
						EnemyStatus = Wandering;
						EnemyWander();
						break;
					}
					EnemyStatusTimer = 0;
				}
				if ((Player->camLocation - GetActorLocation()).Length() < GM->GameManager->EnemySuspiciousDistance)		//If player is close, enemy becomes suspicious
				{
					EnemyStatus = Suspicious;
					EnemyStatusTimer = 0.0f;
				}
			}
			else if (EnemyStatus == Suspicious)
			{
				EnemySuspicious();
				if ((Player->camLocation - GetActorLocation()).Length() < GM->GameManager->EnemyChargeDistance)		//If enemy is suspicious and player is close, enemy attacks.
				{
					EnemyStatus = Charging;
				}
			}
			else if (EnemyStatus == Charging)
			{
				EnemyCharging();
			}
		}
	}
}
