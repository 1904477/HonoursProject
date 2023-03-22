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
	SetActorScale3D(FVector(0.6, 0.6, 0.6));			//Scale Enemy

	auto PTemp = GetWorld()->GetFirstPlayerController()->GetPawn();		
	Player = Cast<ACustomARPawn>(PTemp);

	GetMesh()->BodyInstance.bLockYRotation = true;		//Lock rotation so that characters do not rotate in other directions
	GetMesh()->BodyInstance.bLockXRotation = true;

	EnemyStatus = Idle;		//Starting status is idle.
	WanderRadius = 500.0f;		//Radius for wander area.
	StateSwitchTimer = 2.5f;	//How long before the enemy switches state.
	BoxColor = FColor::White;	

	AIController = Cast<AAIController>(GetController());
	NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIController);
	
}

void ASpawnedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EnemyStatusManager();
	DrawDebugBox(GetWorld(), this->GetRootComponent()->GetComponentLocation(), FVector(10, 10, 30), BoxColor, false, 0.0f, 0, 1.17);

	if (Health <= 0)
	{
		Destroy();
		int tmp = GS->GetScore();
		GS->SetScore(tmp += 10);
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
	AIController->MoveToLocation(Player->camLocation, -1, true, true);
}

void ASpawnedEnemy::ClosestObstacleChecker()
{

}

void ASpawnedEnemy::EnemyStatusManager()
{
	if (AIController)
	{
		if(EnemyStatus !=Suspicious&&EnemyStatus!= Charging&&EnemyStatus!=Attacking)
		{
			EnemyStatusTimer += GetWorld()->GetDeltaSeconds();
			int randomChoice = FMath::RandRange(1, 2);		//Random choice in Enemy Finite State Machine
			if (EnemyStatusTimer > StateSwitchTimer)		//Enemies change state every x seconds (unless suspicious, then timer is longer)
			{
				switch (randomChoice)
				{
				case 1: EnemyStatus = Idle;
					GetCharacterMovement()->MaxWalkSpeed = 0.0f; 
					BoxColor = FColor::White;
					break;
				case 2:
					EnemyStatus = Wandering;
					GetCharacterMovement()->MaxWalkSpeed = 40.f; 
					BoxColor = FColor::Blue;
					EnemyWander();
					break;
				}
				EnemyStatusTimer = 0;
			}
			if ((Player->camLocation - GetActorLocation()).Length() < GM->GameManager->EnemySuspiciousDistance)		//If player is close, enemy becomes suspicious
			{
				EnemyStatus = Suspicious;
				BoxColor = FColor::Orange;
				EnemyStatusTimer = 0.0f;
				GetCharacterMovement()->MaxWalkSpeed = 50.0f; // replace 300 with your desired speed()
			}
		}
		else if(EnemyStatus == Suspicious)
		{
			EnemySuspicious();
			if ((Player->camLocation - GetActorLocation()).Length() < GM->GameManager->EnemyChargeDistance)		//If enemy is suspicious and player is close, enemy attacks.
			{
				GetCharacterMovement()->MaxWalkSpeed = 60.0f;		//In attack state, enemy is faster
				EnemyStatus = Charging;
				BoxColor = FColor::Red;
			}
		}
		else if (EnemyStatus == Charging)
		{
			EnemyCharging();
		}
	}
}
