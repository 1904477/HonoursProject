// Fill out your copyright notice in the Description page of Project Settings.
#include "PoissonSampler.h"
#include "CustomARPawn.h"
#include "CustomGameMode.h"
#include "GameManager.h"

// Sets default values for this component's properties
UPoissonSampler::UPoissonSampler()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UPoissonSampler::BeginPlay()
{
    Super::BeginPlay();

    auto GM = GetWorld()->GetAuthGameMode();        //Get gamemode
    ACustomGameMode* CustomGameMode = Cast<ACustomGameMode>(GM);

    GeneratePoisson(CustomGameMode->GameManager->MainPointsMinDist,     //Generate poisson points as soon as component is created. Necessary variables are passed in, obtained from
                    CustomGameMode->GameManager->SecPointsMaxDist,      //Game manager.
                    CustomGameMode->GameManager->MainPointsSpawnNum, 
                    CustomGameMode->GameManager->SecPointsSpawnNum,
                    CustomGameMode->GameManager->MinDistToPlayer
                    );

    for (int i = 0; i < MainPoints.Num(); i++)      //Draw debug spheres to know where main points are.
        DrawDebugSphere(GetWorld(), FVector(MainPoints[i].X, MainPoints[i].Y, 15),80, 1, FColor(181, 0, 0), false, 10.0f, 0, 2);

    for (int i = 0; i < SecondaryPoints.Num(); i++)     //Draw debug spheres to know where secondary points are.
        DrawDebugSphere(GetWorld(), FVector(SecondaryPoints[i].X, SecondaryPoints[i].Y, 15), 80, 1, FColor::Cyan, false, 10.0f, 0, 2);

}

// Called every frame
void UPoissonSampler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TArray<FVector2f> UPoissonSampler::GeneratePoisson(float minDistMainPoints,float minDistSecPoints, int new_points_count,int secondary_points, float distToPlayer)
{
    NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());        //Get navmesh from the world.
    FNavLocation FirstRandomSpawnPosNavLoc;        //Variable to store random location in navmesh.
    FVector FirstSpawnPos;      //First spawn position.
    if (NavigationArea->GetRandomPointInNavigableRadius(FVector(0, 0, 0), 2000, FirstRandomSpawnPosNavLoc))     //Get random position in navmesh
    {
        // if we were successfull in finding a new location...
        FirstSpawnPos = FirstRandomSpawnPosNavLoc.Location;		//Save first random position in navmesh in FVector.
    }

    MainPoints.Push(FVector2f(FirstSpawnPos.X, FirstSpawnPos.Y));        //Push first point into array of main points, Vector2f as we do not care of height.
    for (int i = 0; i < new_points_count; i++)      //From first point, generate other N points.
    {
        bool isTooClose = true;     //Track if the point generated is too close, true by default.
        FNavLocation RandomSpawnPosNavLoc;     //Variable to store random navmesh location.
        FVector SpawnPos;       //Temporary point spawn location as an FVector.
        while (isTooClose == true)  //Do this until the generated point is far enough from other points.
        {
            if (NavigationArea->GetRandomPointInNavigableRadius(FVector(0, 0, 0), 2000, RandomSpawnPosNavLoc)) //Get random position in navmesh.
            {
                // if we were successfull in finding a new location...
                SpawnPos = RandomSpawnPosNavLoc.Location;		//Save random position in navmesh in FVector.
            }
            if (!inNeighbourhood(FVector2f(RandomSpawnPosNavLoc.Location.X, RandomSpawnPosNavLoc.Location.Y), minDistMainPoints, distToPlayer))       //If the generated point is not too close to other points.
            {
                isTooClose = false;     
            }
        }
        MainPoints.Push(FVector2f(RandomSpawnPosNavLoc.Location.X, RandomSpawnPosNavLoc.Location.Y));       //Push randomly generated point into array of main points.
    }

    for (int i = 0; i < MainPoints.Num(); i++)      //For each main point.
    {
        int pointCount = FMath::RandRange(1, secondary_points);     //Generate a random range of secondary spawning points.
        for (int j = 0; j < secondary_points; j++)      //For each secondary point.
        {
            bool isClose = false;       //Track if the point generated is close enough to other points, false by default.
            FNavLocation RandomSpawnPosNavLoc = FNavLocation();
            FVector SpawnPos;
            while (isClose == false)        //Generate the secondary point until the generated point is close enough to the main point.
            {
                if (NavigationArea->GetRandomPointInNavigableRadius(FVector(0, 0, 0), 2000, RandomSpawnPosNavLoc)) //Get random position in navmesh
                {
                    // if we were successfull in finding a new location...
                    SpawnPos = RandomSpawnPosNavLoc.Location;		//Save random position in navmesh in FVector
                }
               if (FVector2f::Distance((FVector2f(RandomSpawnPosNavLoc.Location.X, RandomSpawnPosNavLoc.Location.Y)),MainPoints[i])< minDistSecPoints)  //If the secondary point is close enough to the main point.
                    isClose = true;
            }
            SecondaryPoints.Push(FVector2f(RandomSpawnPosNavLoc.Location.X, RandomSpawnPosNavLoc.Location.Y));      //Push secondary point into secondary points array.
        }
    }
    return SecondaryPoints;     //Obtain secondary points to spawn enemies from.
}

bool UPoissonSampler::inNeighbourhood(FVector2f point, float mindist, float distToPlayer)       //Calculates if point passed is close to main points.
{
    auto PTemp = GetWorld()->GetFirstPlayerController()->GetPawn();
    ACustomARPawn* Player = Cast<ACustomARPawn>(PTemp);
   //get the neighbourhood if the point in the grid
    for (auto& It : MainPoints)
    {
        //Check if current plane exists 
        if (FVector2f::Distance(point, It)<mindist&& ( Player->camLocation- FVector(It.X,It.Y,5)).Length() < distToPlayer)
        {
            return true;
        }
    }
    return false;
}