// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjectsSpawner.h"
#include "CustomGameMode.h"
#include "CustomGameState.h"
#include "HelloARManager.h"
#include "ARPlaneActor.h"
#include "GunPickup.h"
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

	FTransform tr;		//identity for Poisson component position
	tr.SetIdentity();

	PoissonSampler = Cast<UPoissonSampler>(this->AddComponentByClass(UPoissonSampler::StaticClass(), false, tr, true));		//Add poisson sampler as component.
	PoissonSampler->RegisterComponent();

	if (GameState->SessionModeSelected == VirtualObstacles)
	{
		SpawnVirtualObstacles();
	}
	if (!isGunSpawned)
	{
		SpawnGun();
		isGunSpawned = true;
	}
}

// Called every frame
void AGameObjectsSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GameState->GetHasGameStarted() == true)
	{
		EnemiesSpawnerManager();
	}
}

void AGameObjectsSpawner::EnemiesSpawner()
{
	int randSpawnPoint= FMath::RandRange(0, PoissonSampler->SecondaryPoints.Num()-1);;

	FVector spawnPos = PoissonSampler->SecondaryPoints[randSpawnPoint];

	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	ASpawnedEnemy* Enemy = GetWorld()->SpawnActor<ASpawnedEnemy>(EnemyToSpawn, FVector(spawnPos.X, spawnPos.Y, spawnPos.Z+25), MyRot, SpawnInfo);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, Enemy->GetActorLocation().ToString());

	Enemies.Add(Enemy);
}

void AGameObjectsSpawner::EnemiesSpawnerManager()
{
	if (EnemySpawnTimer > CustomGameMode->GameManager->EnemiesSpawnTimer && Enemies.Num() <= CustomGameMode->GameManager->EnemiesToSpawn && GameState->GetHasGameStarted()==true)
	{
		EnemiesSpawner();
		EnemySpawnTimer = 0;
	}
	else if(EnemySpawnTimer <=3 && GameState->GetHasGameStarted() == true)		//If spawntimer is less than three and the game has started
	EnemySpawnTimer += GetWorld()->GetDeltaSeconds();		//Update enemy spawn timer.
}

void AGameObjectsSpawner::SpawnVirtualObstacles()
{
	SpawnTables();
	SpawnSteps();
}

void AGameObjectsSpawner::SpawnTables()
{
	int tableDistanceX = 220;
	int tableDistanceY = 220;
	int tableAngle = 90;
	FVector spawnPos;
	for (int i = 0; i < 4; i++)	//4 Tables
	{
		if (i == 0)
			spawnPos = FVector(0, tableDistanceY, 0);
		else if (i == 1)
			spawnPos = FVector(0, -tableDistanceY, 0);
		else if (i == 2)
		{
			spawnPos = FVector(tableDistanceX, 0, 0);
			tableAngle = 0;
		}
		else if (i == 3)
		{
			spawnPos = FVector(-tableDistanceX, 0, 0);
			tableAngle = 0;
		}
		const FActorSpawnParameters SpawnInfo;
		const FRotator MyRot(0, tableAngle, 0);
		ATableObstacle* Table = GetWorld()->SpawnActor<ATableObstacle>(spawnPos, MyRot, SpawnInfo);

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
		AStepObstacle* Step = GetWorld()->SpawnActor<AStepObstacle>(spawnPos, MyRot, SpawnInfo);

		Steps.Add(Step);
	}
}

void AGameObjectsSpawner::SpawnGun()
{
	if (UGameplayStatics::GetPlatformName() == "IOS" || UGameplayStatics::GetPlatformName() == "Android")
	{
		FVector tablePos = CustomGameMode->ARManager->FirstTable->GetActorLocation();
		const FActorSpawnParameters SpawnInfo;
		const FRotator MyRot(0, 0, 0);
		AGunPickup* Gun = GetWorld()->SpawnActor<AGunPickup>(GunToSpawn, FVector(tablePos.X, tablePos.Y, tablePos.Z), MyRot, SpawnInfo);
		//AGunPickup* Gun = GetWorld()->SpawnActor<AGunPickup>(GunToSpawn, FVector(5,5,5), MyRot, SpawnInfo);
	}
	else
	{
		const FActorSpawnParameters SpawnInfo;
		const FRotator MyRot(0, 0, 0);
		AGunPickup* Gun = GetWorld()->SpawnActor<AGunPickup>(GunToSpawn, FVector(5,5,5), MyRot, SpawnInfo);
	}
}
