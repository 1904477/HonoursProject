// Fill out your copyright notice in the Description page of Project Settings.
#include "PoissonSampler.h"

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

    auto GM = GetWorld()->GetAuthGameMode();
    ACustomGameMode* CustomGameMode = Cast<ACustomGameMode>(GM);

    //GeneratePoisson(1500,200, 4,5);
    GeneratePoisson(CustomGameMode->GameManager->MainPointsMinDist, 
                    CustomGameMode->GameManager->SecPointsMaxDist, 
                    CustomGameMode->GameManager->MainPointsSpawnNum, 
                    CustomGameMode->GameManager->SecPointsSpawnNum);

    for (int i = 0; i < MainPoints.Num(); i++)
        DrawDebugSphere(GetWorld(), FVector(MainPoints[i].X, MainPoints[i].Y, 15), 50, 1, FColor(181, 0, 0), false, 10.0f, 0, 2);

    for (int i = 0; i < SecondaryPoints.Num(); i++)
        DrawDebugSphere(GetWorld(), FVector(SecondaryPoints[i].X, SecondaryPoints[i].Y, 15), 50, 1, FColor::Cyan, false, 10.0f, 0, 2);

}


// Called every frame
void UPoissonSampler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TArray<FVector2f> UPoissonSampler::GeneratePoisson(float minDistMainPoints,float minDistSecPoints, int new_points_count,int secondary_points)
{
    //Create the grid

    NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    FNavLocation FirstRandomSpawnPosNavLoc = FNavLocation();
    FVector FirstSpawnPos;
    if (NavigationArea->GetRandomPointInNavigableRadius(FVector(0, 0, 0), 2000, FirstRandomSpawnPosNavLoc)) //Get random position in navmesh
    {
        // if we were successfull in finding a new location...
        FirstSpawnPos = FirstRandomSpawnPosNavLoc.Location;		//Save random position in navmesh in FVector
    }

    FVector2f firstPoint(FirstSpawnPos.X, FirstSpawnPos.Y);

    MainPoints.Push(firstPoint);


    for (int i = 0; i < new_points_count; i++)
    {
        bool isTooClose = true;
        FNavLocation RandomSpawnPosNavLoc = FNavLocation();
        FVector SpawnPos;
        FVector2f tempPos;
        while (isTooClose == true)
        {
            if (NavigationArea->GetRandomPointInNavigableRadius(FVector(0, 0, 0), 2000, RandomSpawnPosNavLoc)) //Get random position in navmesh
            {
                // if we were successfull in finding a new location...
                SpawnPos = RandomSpawnPosNavLoc.Location;		//Save random position in navmesh in FVector
            }
            tempPos=FVector2f(RandomSpawnPosNavLoc.Location.X, RandomSpawnPosNavLoc.Location.Y);

            if (!inNeighbourhood(tempPos, minDistMainPoints))
            {
                isTooClose = false;
            }
        }
        MainPoints.Push(tempPos);
    }

    for (int i = 0; i < MainPoints.Num(); i++)
    {
        int pointCount = FMath::RandRange(3, secondary_points);
        for (int j = 0; j < secondary_points; j++)
        {
            bool isClose = false;
            FNavLocation RandomSpawnPosNavLoc = FNavLocation();
            FVector SpawnPos;
            FVector2f tempPos;
            while (isClose == false)
            {
                if (NavigationArea->GetRandomPointInNavigableRadius(FVector(0, 0, 0), 2000, RandomSpawnPosNavLoc)) //Get random position in navmesh
                {
                    // if we were successfull in finding a new location...
                    SpawnPos = RandomSpawnPosNavLoc.Location;		//Save random position in navmesh in FVector
                }
                tempPos = FVector2f(RandomSpawnPosNavLoc.Location.X, RandomSpawnPosNavLoc.Location.Y);

                if (FVector2f::Distance((tempPos),MainPoints[i])< minDistSecPoints)
                {
                    isClose = true;
                }
            }
            SecondaryPoints.Push(tempPos);
        }
    }


    return SecondaryPoints;
}

bool UPoissonSampler::inNeighbourhood(FVector2f point, float mindist)
{
   //get the neighbourhood if the point in the grid
    for (auto& It : MainPoints)
    {
        //Check if current plane exists 
        if (FVector2f::Distance(point, It)<mindist)
        {
            return true;
        }
    }
    return false;
}