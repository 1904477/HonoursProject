// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjectsSpawner.h"
#include "CustomGameState.h"
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
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Spawned game object spawner"));

	auto GS = GetWorld()->GetGameState();
	GameState = Cast<ACustomGameState>(GS);
}

// Called every frame
void AGameObjectsSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EnemiesSpawnerManager();
}


void AGameObjectsSpawner::EnemiesSpawner()
{
	float posX = 2200;
	float posY = 2200;
	posX = FMath::RandRange(-posX, posX);
	posY = FMath::RandRange(-posY, posY);
	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	const FVector MyLoc(posX, posY, 0);

	APlaceablePlayer* Player = GetWorld()->SpawnActor<APlaceablePlayer>(PlacableToSpawn, MyLoc, MyRot, SpawnInfo);

	Enemies.Add(Player);
}

void AGameObjectsSpawner::EnemiesSpawnerManager()
{
	if (EnemySpawnTimer > 3 && Enemies.Num() <= 10&& GameState->GetHasGameStarted()==true)
	{
		EnemiesSpawner();
		EnemySpawnTimer = 0;
	}
	else if(EnemySpawnTimer <=3 && GameState->GetHasGameStarted() == true)
	EnemySpawnTimer += GetWorld()->GetDeltaSeconds();
}
