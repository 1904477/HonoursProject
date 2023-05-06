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

	//Get Gamestate.
	auto GS = GetWorld()->GetGameState();		
	GameState = Cast<ACustomGameState>(GS);
	//Get Gamemode.
	auto GM = GetWorld()->GetAuthGameMode();
	CustomGameMode = Cast<ACustomGameMode>(GM);
	//Get CustomARPawn.
	auto P = GetWorld()->GetFirstPlayerController()->GetPawn();
	Player = Cast<ACustomARPawn>(P);

	FTransform tr;		//identity for Poisson component position
	tr.SetIdentity();

	//Add PoissonSampler as component.
	PoissonSampler = Cast<UPoissonSampler>(this->AddComponentByClass(UPoissonSampler::StaticClass(), false, tr, true));		
	PoissonSampler->RegisterComponent();

	//SpawnGraves in secondary points.
	SpawnGraves();

	//Spawn virtual obstacles only when the level selected is the virtual one.
	if (GetWorld()->GetMapName()=="VirtualObstaclesLevel"|| UGameplayStatics::GetPlatformName()=="Windows")
	{
		SpawnVirtualObstacles();
	}
	//Spawn gun.
	if (!isGunSpawned)
	{
		SpawnGun();
		isGunSpawned = true;
	}
	//Spawn Hatch.
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

	//If game has started, call EnemiesSpawnerManager.
	if (GameState->GetHasGameStarted() == true)
		EnemiesSpawnerManager();
	
	//If there is at least one enemy in the scene.
	if (Enemies.Num() > 0)
	{
		//Get closest enemy for enemy-close icon.
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
	//Get a random spawning point in the range from zero to the number of secondary points for zombie spawn.
	int randSpawnPoint= FMath::RandRange(0, PoissonSampler->SecondaryPoints.Num()-1);;

	//Set spawn pos to be at the secondary point indexed by the random value previously generated.
	FVector spawnPos = PoissonSampler->SecondaryPoints[randSpawnPoint];

	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	//Spawn enemy, add it to the enemy array, reset spawning timer and reduce remaining zombies to spawn.
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
	//If there are enemies to spawn.
	if (CustomGameMode->GameManager->EnemiesToSpawn !=0)
	{
		//If game has started, spawn enemies.
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
	//Function to spawn tables.
	int tableDistanceX = 150;
	int tableDistanceY = 150;
	int tableAngle = 90;
	FVector spawnPos;
	//Spawn two tables.
	for (int i = 0; i < 2; i++)	
	{
		if (i == 0)
			spawnPos = FVector(Player->camLocation.X, Player->camLocation.Y + tableDistanceY, 0);
		else if (i == 1)
			spawnPos = FVector(Player->camLocation.X, Player->camLocation.Y - tableDistanceY, 0);

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
	//Spawn five steps.
	for (int i = 0; i < 5; i++)	
	{
		switch (i)
		{
		case 0:
			spawnPos = FVector(Player->camLocation.X-205, Player->camLocation.Y-120, 0);
			StepAngle = -45;
			break;
		case 1:
			spawnPos = FVector(Player->camLocation.X+ 130, Player->camLocation.Y-330, 0);
			StepAngle = 60;
			break;

		case 2:
			spawnPos = FVector(Player->camLocation.X-180,Player->camLocation.Y+ 270, 0);
			StepAngle = 0;
			break;

		case 3:
			spawnPos = FVector(Player->camLocation.X+460, Player->camLocation.Y-70, 0);
			StepAngle = -45;
			break;

		case 4:
			spawnPos = FVector(Player->camLocation.X+290, Player->camLocation.Y + 380, 0);
			StepAngle = 45;
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
	//Spawn graves in main points.
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
		//Spawn gun on virtual table if level is virtual obstacles. 
		if (GetWorld()->GetMapName() == "VirtualObstaclesLevel")
		{
			FVector tablePos = Tables[0]->GetActorLocation();
			const FActorSpawnParameters SpawnInfo;
			const FRotator MyRot(0, 0, 0);
			AGunPickup* Gun = GetWorld()->SpawnActor<AGunPickup>(GunToSpawn, FVector(tablePos.X, tablePos.Y, tablePos.Z), MyRot, SpawnInfo);
		}
		//Spawn gun on first real table if level is virtual obstacles. 
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
	//Spawn navmesh in a random position of the floor.
	if (UGameplayStatics::GetPlatformName() == "IOS" || UGameplayStatics::GetPlatformName() == "Android")
	{
		if (NavigationArea)
		{
			//While loop to make sure that the position on the navmesh is being generated until the position is on the floor, and not any higher.
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
