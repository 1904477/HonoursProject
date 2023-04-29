// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjectsSpawner.h"
#include "CustomGameMode.h"
#include "CustomGameState.h"
#include "HelloARManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GraveObject.h"
#include "ARPlaneActor.h"
#include "GunPickup.h"
#include "CustomARPawn.h"
#include "GameManager.h"
// Sets default values
AGameObjectsSpawner::AGameObjectsSpawner() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameObjectsSpawner::BeginPlay()
{
	Super::BeginPlay();

	auto GS = GetWorld()->GetGameState();		//Get gamestate and gamemode
	GameState = Cast<ACustomGameState>(GS);
	auto GM = GetWorld()->GetAuthGameMode();
	CustomGameMode = Cast<ACustomGameMode>(GM);
	auto P = GetWorld()->GetFirstPlayerController()->GetPawn();
	Player = Cast<ACustomARPawn>(P);

	FTransform tr;		//identity for Poisson component position
	tr.SetIdentity();

	PoissonSampler = Cast<UPoissonSampler>(this->AddComponentByClass(UPoissonSampler::StaticClass(), false, tr, true));		//Add poisson sampler as component.
	PoissonSampler->RegisterComponent();
	SpawnGraves();

	if (GetWorld()->GetMapName()=="VirtualObstaclesLevel"|| UGameplayStatics::GetPlatformName()=="Windows")
	{
		SpawnVirtualObstacles();
	}
	if (!isGunSpawned)
	{
		SpawnGun();
		isGunSpawned = true;
	}
	if (!isHatchSpawned)	
	{
		SpawnHatch();
		isHatchSpawned = true;

	}
}

// Called every frame
void AGameObjectsSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GameState->GetHasGameStarted() == true)
		EnemiesSpawnerManager();
	if (Enemies.Num() > 0)
	{
		GameState->SetIsIsIsOneEnemyAlive(true);
		TArray<AActor*> asActs;
		float dist_ = 0.0f;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnedEnemy::StaticClass(), asActs);
		UGameplayStatics::FindNearestActor(Player->GetActorLocation(), asActs, dist_);
		
		if (dist_ < CustomGameMode->GameManager->IsEnemyTooCloseDistance)
		{
			GameState->SetIsEnemyTooClose(true);
		}
		else
		{
			GameState->SetIsEnemyTooClose(false);
		}
	}
	else
		GameState->SetIsIsIsOneEnemyAlive(false);
}

void AGameObjectsSpawner::EnemiesSpawner()
{
	int randSpawnPoint= FMath::RandRange(0, PoissonSampler->SecondaryPoints.Num()-1);;

	FVector spawnPos = PoissonSampler->SecondaryPoints[randSpawnPoint];

	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	ASpawnedEnemy* Enemy = GetWorld()->SpawnActor<ASpawnedEnemy>(EnemyToSpawn, FVector(spawnPos.X, spawnPos.Y, spawnPos.Z+50), MyRot, SpawnInfo);
	if (Enemy)
	{
		Enemies.Add(Enemy);
		CustomGameMode->GameManager->EnemiesToSpawn--;
		EnemySpawnTimer = 0;
	}
}

void AGameObjectsSpawner::EnemiesSpawnerManager()
{
	if (CustomGameMode->GameManager->EnemiesToSpawn !=0)
	{
		if (EnemySpawnTimer > CustomGameMode->GameManager->EnemiesSpawnTimer && GameState->GetHasGameStarted() == true)
		{
			EnemiesSpawner();
		}
		else if (EnemySpawnTimer <= CustomGameMode->GameManager->EnemiesSpawnTimer && GameState->GetHasGameStarted() == true)		//If spawntimer is less than three and the game has started
			EnemySpawnTimer += GetWorld()->GetDeltaSeconds();		//Update enemy spawn timer.
	}
}

void AGameObjectsSpawner::SpawnVirtualObstacles()
{
	SpawnTables();
	SpawnSteps();
}

void AGameObjectsSpawner::SpawnTables()
{
	int tableDistanceX = 190;
	int tableDistanceY = 190;
	int tableAngle = 90;
	FVector spawnPos;
	for (int i = 0; i < 4; i++)	//4 Tables
	{
		if (i == 0)
			spawnPos = FVector(Player->GetActorLocation().X, Player->GetActorLocation().Y + tableDistanceY, 0);
		else if (i == 1)
			spawnPos = FVector(Player->GetActorLocation().X, Player->GetActorLocation().Y - tableDistanceY, 0);
		else if (i == 2)
		{
			spawnPos = FVector(Player->GetActorLocation().X + tableDistanceX, Player->GetActorLocation().Y, 0);
			tableAngle = 0;
		}
		else if (i == 3)
		{
			spawnPos = FVector(Player->GetActorLocation().X - tableDistanceX, Player->GetActorLocation().Y, 0);
			tableAngle = 0;
		}
		const FActorSpawnParameters SpawnInfo;
		const FRotator MyRot(0, tableAngle, 0);
		ATableObstacle* Table = GetWorld()->SpawnActor<ATableObstacle>(TableObstacleToSpawn, spawnPos, MyRot, SpawnInfo);

		Tables.Add(Table);
	}
}

void AGameObjectsSpawner::SpawnSteps()
{
	int StepAngle = 90;
	FVector spawnPos;
	for (int i = 0; i < 6; i++)	//4 Tables
	{
		switch (i)
		{
		case 0:
			spawnPos = FVector(-205, -440, 0);
			StepAngle = -45;
			break;
		case 1:
			spawnPos = FVector(130, -450, 0);
			StepAngle = 60;
			break;

		case 2:
			spawnPos = FVector(-490, 110, 0);
			StepAngle = 0;
			break;

		case 3:
			spawnPos = FVector(460, -70, 0);
			StepAngle = -45;
			break;

		case 4:
			spawnPos = FVector(290, 380, 0);
			StepAngle = 45;
			break;

		case 5:
			spawnPos = FVector(-200, 500, 0);
			StepAngle = 80;
			break;
		default:
			break;
		}
		const FActorSpawnParameters SpawnInfo;
		const FRotator MyRot(0, StepAngle, 0);
		AStepObstacle* Step = GetWorld()->SpawnActor<AStepObstacle>(StepObstacleToSpawn, spawnPos, MyRot, SpawnInfo);

		Steps.Add(Step);
	}
}

void AGameObjectsSpawner::SpawnGraves()
{
	for (int i = 0; i < PoissonSampler->MainPoints.Num(); i++)
	{
		const FActorSpawnParameters SpawnInfo;
		FVector spawnPos = PoissonSampler->MainPoints[i];
		FRotator MyRot;
		AGraveObject* Grave = GetWorld()->SpawnActor<AGraveObject>(spawnPos, MyRot, SpawnInfo);

		Graves.Add(Grave);
	}
}

void AGameObjectsSpawner::SpawnGun()
{
	if (UGameplayStatics::GetPlatformName() == "IOS" || UGameplayStatics::GetPlatformName() == "Android")
	{
		if (GetWorld()->GetMapName() == "VirtualObstaclesLevel")
		{
			FVector tablePos = Tables[0]->GetActorLocation();
			const FActorSpawnParameters SpawnInfo;
			const FRotator MyRot(0, 0, 0);
			AGunPickup* Gun = GetWorld()->SpawnActor<AGunPickup>(GunToSpawn, FVector(tablePos.X, tablePos.Y, tablePos.Z), MyRot, SpawnInfo);
		}
		else
		{
			FVector tablePos = CustomGameMode->ARManager->FirstTable->GetActorLocation();
			const FActorSpawnParameters SpawnInfo;
			const FRotator MyRot(0, 0, 0);
			AGunPickup* Gun = GetWorld()->SpawnActor<AGunPickup>(GunToSpawn, FVector(tablePos.X, tablePos.Y, tablePos.Z), MyRot, SpawnInfo);
			//AGunPickup* Gun = GetWorld()->SpawnActor<AGunPickup>(GunToSpawn, FVector(5,5,5), MyRot, SpawnInfo);
		}
	}
	else
	{
		FVector tablePos = Tables[0]->GetActorLocation();
		const FActorSpawnParameters SpawnInfo;
		const FRotator MyRot(0, 0, 0);
		AGunPickup* Gun = GetWorld()->SpawnActor<AGunPickup>(GunToSpawn, FVector(tablePos.X, tablePos.Y, tablePos.Z), MyRot, SpawnInfo);
	}
}

void AGameObjectsSpawner::SpawnHatch()
{
	NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());        //Get navmesh from the world.
	FNavLocation RandomSpawnPosNavLoc;
	FVector SpawnPos;
	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	if (UGameplayStatics::GetPlatformName() == "IOS" || UGameplayStatics::GetPlatformName() == "Android")
	{
	
		if (NavigationArea)
		{
			while (isHatchOnGround == false)
			{
				if (PoissonSampler->NavigationArea->GetRandomPointInNavigableRadius(FVector(0, 0, 0), 2000, RandomSpawnPosNavLoc)) //Get random position in navmesh
				{
					// if we were successfull in finding a new location...
					SpawnPos = RandomSpawnPosNavLoc.Location;		//Save random position in navmesh in FVector
				}
				if (SpawnPos.Z < CustomGameMode->ARManager->LowestPlaneActor->GetActorLocation().Z + 45)
				{
					Hatch = GetWorld()->SpawnActor<AHatch>(HatchToSpawn, SpawnPos, MyRot, SpawnInfo);
					isHatchOnGround = true;
				}
			}
			

		}
	}
	else
	Hatch = GetWorld()->SpawnActor<AHatch>(HatchToSpawn, SpawnPos, MyRot, SpawnInfo);
}
