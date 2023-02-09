// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceablePlayer.h"
#include "GameManager.h"
#include "CustomGameMode.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
APlaceablePlayer::APlaceablePlayer()
{
	PrimaryActorTick.bCanEverTick = true;


	myCols.Add(FColor::Green);
	myCols.Add(FColor::Yellow);
	myCols.Add(FColor::Purple);
	myCols.Add(FColor::Red);
	myCols.Add(FColor::Cyan);
	myCols.Add(FColor::Blue);


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

	colChangetmr += DeltaTime;

	Super::Tick(DeltaTime);
	EnemyStatusManager();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::FromInt(enemyStatus));

	DrawDebugBox(GetWorld(), this->GetRootComponent()->GetComponentLocation(), FVector(50, 50, 150), myCols[idxCol% myCols.Num()], false, 0.0f, 0, 3.17);

	if (colChangetmr > 1.17) {
		idxCol++;
		colChangetmr = 0;
	}

}

// Called to bind functionality to input
void APlaceablePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlaceablePlayer::EnemySuspicious()
{
	suspiciousTimer += GetWorld()->GetDeltaSeconds();
	if (suspiciousTimer > 3.0f) {
		suspiciousTimer = 0;
		enemyStatus = Idle;
	}

	moveTo = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	AIController->MoveToActor(GetWorld()->GetFirstPlayerController()->GetPawn(), -1,true,true);

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
		if(enemyStatus !=Suspicious)
		{
			enemyStatusTimer += GetWorld()->GetDeltaSeconds();
			int randomChoice = FMath::RandRange(1, 2);		//Random choice
			if (enemyStatusTimer > 1.3f)		//Enemies change state every x seconds (unless suspicious, then timer is longer)
			{
				switch (randomChoice)
				{
				case 1: enemyStatus = Idle;
					GetCharacterMovement()->MaxWalkSpeed = 0.0f; // replace 300 with your desired speed()
					break;
				case 2:
					enemyStatus = Wandering;
					GetCharacterMovement()->MaxWalkSpeed = 180.f; // replace 300 with your desired speed()
					EnemyWander();
					break;
				}
				enemyStatusTimer = 0;
			}
			if (abs((GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation()).Length()) < 800)		//If player is close, enemy becomes suspicious
			{
				enemyStatus = Suspicious;
				enemyStatusTimer = 0.0f;
				GetCharacterMovement()->MaxWalkSpeed = 250.0f; // replace 300 with your desired speed()
			}
		}
		else if(enemyStatus == Suspicious)
		{
			EnemySuspicious();
		}
	}
}
