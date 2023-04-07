// Fill out your copyright notice in the Description page of Project Settings.
#include "PoissonSampler.h"
#include "CustomARPawn.h"
#include "CustomGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameManager.h"
#include "HelloARManager.h"
#include "ARPlaneActor.h"
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
    CustomGameMode = Cast<ACustomGameMode>(GM);
    auto PTemp = GetWorld()->GetFirstPlayerController()->GetPawn();
    Player = Cast<ACustomARPawn>(PTemp);

    IsSecondaryPointClose = false;
    IsSecondaryPointHigh = false;
    isMainPointClose = true;
    isMainPointHigh = true;

    GeneratePoisson(CustomGameMode->GameManager->MainPointsMinDist,     //Generate poisson points as soon as component is created. Necessary variables are passed in, obtained from
                    CustomGameMode->GameManager->SecPointsMaxDist,      //Game manager.
                    CustomGameMode->GameManager->MainPointsSpawnNum, 
                    CustomGameMode->GameManager->SecPointsSpawnNum,
                    CustomGameMode->GameManager->MinDistToPlayer
                    );
    for (int i = 0; i < MainPoints.Num(); i++)      //Draw debug spheres to know where main points are.
        DrawDebugSphere(GetWorld(), FVector(MainPoints[i].X, MainPoints[i].Y, MainPoints[i].Z), 150, 1, FColor(181, 0, 0), false, 10.0f, 0, 2);

    for (int i = 0; i < SecondaryPoints.Num(); i++)     //Draw debug spheres to know where secondary points are.
        DrawDebugSphere(GetWorld(), FVector(SecondaryPoints[i].X, SecondaryPoints[i].Y, SecondaryPoints[i].Z), 150, 1, FColor::Cyan, false, 10.0f, 0, 2);
}

// Called every frame
void UPoissonSampler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, FString::SanitizeFloat(MainPoints.Num()));

}

FVector UPoissonSampler::MainPointsGeneration(float minDistMainPoints, int new_points_count, float distToPlayer,std::string pointType)
{
    FVector SpawnPos;
    isMainPointHigh = true;
    isMainPointClose = true;
    FNavLocation RandomSpawnPosNavLoc;

    while (isMainPointClose == true || isMainPointHigh == true)
    {

        isMainPointHigh = true;
        isMainPointClose = true;
        if (NavigationArea->GetRandomPointInNavigableRadius(FVector(0, 0, 0), 2000, RandomSpawnPosNavLoc))     //Get random position in navmesh
            SpawnPos = RandomSpawnPosNavLoc.Location;		//Save first random position in navmesh in FVector.
        if (pointType == "first")       //If first point is being generated
        {
           if (UGameplayStatics::GetPlatformName() == "IOS" || UGameplayStatics::GetPlatformName() == "Android")
           {
               if (RandomSpawnPosNavLoc.Location.Z < (CustomGameMode->ARManager->LowestPlaneActor->GetActorLocation().Z + 250))  //If the secondary point is close enough to the main point.
                   isMainPointHigh = false;
           }
           //if (RandomSpawnPosNavLoc.Location.Z < 10)  //If the secondary point is close enough to the main point.
           //{
           //    isMainPointHigh = false;
           //}
            if ((RandomSpawnPosNavLoc.Location - Player->camLocation).Length() > distToPlayer)       //Make sure the first point is far enough from the player.
                isMainPointClose = false;
        }


        else if (pointType == "main")       //If the other main points are being generated.
        {
            if (!inNeighbourhood(RandomSpawnPosNavLoc.Location, minDistMainPoints, distToPlayer))       //If the generated point is not too close to other main points and is not too close to the player.
                isMainPointClose = false;
           if (UGameplayStatics::GetPlatformName() == "IOS" || UGameplayStatics::GetPlatformName() == "Android")
           {
               if (RandomSpawnPosNavLoc.Location.Z < (CustomGameMode->ARManager->LowestPlaneActor->GetActorLocation().Z + 50))  //If the secondary point is close enough to the main point.
               {
                   isMainPointHigh = false;
               }
           }
           else
           {
                if (RandomSpawnPosNavLoc.Location.Z < 10)  //If the secondary point is close enough to the main point.
                {
                    isMainPointHigh = false;
                }
            }

        }
    }
    GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Emerald, RandomSpawnPosNavLoc.Location.ToString());

    return RandomSpawnPosNavLoc.Location;
}

void UPoissonSampler::SecondaryPointsGeneration(float minDistSecPoints, int secondary_points)
{
    for (int i = 0; i < MainPoints.Num(); i++)      //For each main point.
    {
        int pointCount = FMath::RandRange(1, secondary_points);     //Generate a random range of secondary spawning points.
        for (int j = 0; j < pointCount; j++)      //For each secondary point.
        {
            FNavLocation RandomSpawnPosNavLocSec;
            RandomSpawnPosNavLocSec.Location = FVector(0, 0, 0);
            FVector SpawnPos;
            IsSecondaryPointClose = false;
            IsSecondaryPointHigh = true;
            while (IsSecondaryPointHigh==true ||RandomSpawnPosNavLocSec.Location == MainPoints[i]|| (RandomSpawnPosNavLocSec.Location - MainPoints[i]).Length()>minDistSecPoints)        //Generate the secondary point until the generated point is close enough to the main point.
            {
                if (NavigationAreaSec->GetRandomPointInNavigableRadius(FVector(0, 0, 0), 2000, RandomSpawnPosNavLocSec)) //Get random position in navmesh
                {
                    // if we were successfull in finding a new location...
                    SpawnPos = RandomSpawnPosNavLocSec.Location;		//Save random position in navmesh in FVector
                }
                if (UGameplayStatics::GetPlatformName() == "IOS" || UGameplayStatics::GetPlatformName() == "Android")
                {
                    if (RandomSpawnPosNavLocSec.Location.Z < (CustomGameMode->ARManager->LowestPlaneActor->GetActorLocation().Z + 50))  //If the secondary point is close enough to the main point.
                        IsSecondaryPointHigh = false;
                }
                else
                {
                    if (RandomSpawnPosNavLocSec.Location.Z < 3)  //If the secondary point is close enough to the main point.
                        IsSecondaryPointHigh = false;
                }
            }

            SecondaryPoints.Push(RandomSpawnPosNavLocSec.Location);      //Push secondary point into secondary points array.
            GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Blue, RandomSpawnPosNavLocSec.Location.ToString());

        }
    }
}

TArray<FVector> UPoissonSampler::GeneratePoisson(float minDistMainPoints,float minDistSecPoints, int new_points_count,int secondary_points, float distToPlayer)
{
    NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());        //Get navmesh from the world.
    NavigationAreaSec = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());        //Get navmesh from the world.

    MainPoints.Push(MainPointsGeneration(minDistMainPoints, new_points_count, distToPlayer, "first"));      //Generate first main point
    
    while(MainPoints.Num()< new_points_count)
    MainPoints.Push(MainPointsGeneration(minDistMainPoints, new_points_count, distToPlayer, "main"));       //Push randomly generated point into array of main points.

    SecondaryPointsGeneration(minDistSecPoints, secondary_points);      //Generate all secondary points
    return SecondaryPoints;     //Obtain secondary points to spawn enemies from.
}

bool UPoissonSampler::inNeighbourhood(FVector point, float mindist, float distToPlayer)       //Calculates if point passed is close to main points.
{
    auto PTemp = GetWorld()->GetFirstPlayerController()->GetPawn();
    Player = Cast<ACustomARPawn>(PTemp);

   //get the neighbourhood if the point in the grid
    for (auto& It : MainPoints)
    {
        //Check if point is too close to other main points or if point is too close to player location.
        if ((point- It).Length() <mindist || (point-Player->camLocation).Length() <  distToPlayer)
            return true;
    }
    return false;
}