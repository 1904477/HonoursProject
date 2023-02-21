// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjectsSpawner.h"
#include "CustomGameMode.h"
#include "CustomGameState.h"
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

	auto GS = GetWorld()->GetGameState();
	GameState = Cast<ACustomGameState>(GS);
	auto GM = GetWorld()->GetAuthGameMode();
	CustomGameMode = Cast<ACustomGameMode>(GM);

	FTransform tr;
	tr.SetIdentity();

	PoissonSampler = Cast<UPoissonSampler>(this->AddComponentByClass(UPoissonSampler::StaticClass(), false, tr, true));
	PoissonSampler->RegisterComponent();
}

// Called every frame
void AGameObjectsSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EnemiesSpawnerManager();
}


void AGameObjectsSpawner::EnemiesSpawner()
{
	FVector spawnPos = FVector(PoissonSampler->SecondaryPoints[0].X, PoissonSampler->SecondaryPoints[0].Y, 5);
	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	APlaceablePlayer* Player = GetWorld()->SpawnActor<APlaceablePlayer>(PlacableToSpawn, spawnPos, MyRot, SpawnInfo);

	Enemies.Add(Player);
}

void AGameObjectsSpawner::EnemiesSpawnerManager()
{
	if (EnemySpawnTimer > CustomGameMode->GameManager->EnemiesSpawnTimer && Enemies.Num() <= CustomGameMode->GameManager->EnemiesToSpawn && GameState->GetHasGameStarted()==true)
	{
		EnemiesSpawner();
		EnemySpawnTimer = 0;
	}
	else if(EnemySpawnTimer <=3 && GameState->GetHasGameStarted() == true)
	EnemySpawnTimer += GetWorld()->GetDeltaSeconds();
}
