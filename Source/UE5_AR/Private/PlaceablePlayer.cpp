// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceablePlayer.h"
#include "GameManager.h"
#include "CustomGameMode.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	//SetActorScale3D(FVector(1.2, 1.2, 1.2));
	EnemyStatus = Idle;
	WanderRadius = 500.0f;
	BoxColor = FColor::White;
	NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIController);

	FNavLocation RandomSpawnPos = FNavLocation(GetActorLocation());


	if (NavigationArea->GetRandomPointInNavigableRadius(GetActorLocation(), 3000, RandomSpawnPos)) //Get random position in navmesh
	{
		// if we were successfull in finding a new location...
		FVector SpawnPos = RandomSpawnPos.Location;		//Save random position in navmesh in FVector
		SetActorLocation(FVector(SpawnPos.X, SpawnPos.Y, SpawnPos.Z+70));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("good pointer"));
	}
}

void APlaceablePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EnemyStatusManager();
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::FromInt(EnemyStatus));
	DrawDebugBox(GetWorld(), this->GetRootComponent()->GetComponentLocation(), FVector(10, 10, 30), BoxColor, false, 0.0f, 0, 1.17);
}

// Called to bind functionality to input
void APlaceablePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlaceablePlayer::EnemySuspicious()
{
	SuspiciousTimer += GetWorld()->GetDeltaSeconds();
	if (SuspiciousTimer > 3.0f) {
		SuspiciousTimer = 0;
		EnemyStatus = Idle;
	}

	MoveTo = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	AIController->MoveToActor(GetWorld()->GetFirstPlayerController()->GetPawn(), -1,true,true);

}

void APlaceablePlayer::EnemyWander()
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
void APlaceablePlayer::EnemyStatusManager()
{
	if (AIController)
	{
		if(EnemyStatus !=Suspicious)
		{
			EnemyStatusTimer += GetWorld()->GetDeltaSeconds();
			int randomChoice = FMath::RandRange(1, 2);		//Random choice
			if (EnemyStatusTimer > 1.3f)		//Enemies change state every x seconds (unless suspicious, then timer is longer)
			{
				switch (randomChoice)
				{
				case 1: EnemyStatus = Idle;
					GetCharacterMovement()->MaxWalkSpeed = 0.0f; // replace 300 with your desired speed()
					BoxColor = FColor::White;

					break;
				case 2:
					EnemyStatus = Wandering;
					GetCharacterMovement()->MaxWalkSpeed = 180.f; // replace 300 with your desired speed()
					BoxColor = FColor::Blue;
					EnemyWander();
					break;
				}
				EnemyStatusTimer = 0;
			}
			if (abs((GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation()).Length()) < 200)		//If player is close, enemy becomes suspicious
			{
				EnemyStatus = Suspicious;
				BoxColor = FColor::Orange;
				EnemyStatusTimer = 0.0f;
				GetCharacterMovement()->MaxWalkSpeed = 250.0f; // replace 300 with your desired speed()
			}
		}
		else if(EnemyStatus == Suspicious)
		{
			EnemySuspicious();
		}
	}
}
