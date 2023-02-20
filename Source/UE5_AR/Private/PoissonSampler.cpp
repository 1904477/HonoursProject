// Fill out your copyright notice in the Description page of Project Settings.


#include "PoissonSampler.h"

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



}


// Called every frame
void UPoissonSampler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

FVector UPoissonSampler::GeneratePoisson(float width, float height, float min_dist, int new_points_count)
{
    //Create the grid
    int cellSize = min_dist / sqrt(2);

    float randX = FMath::FRand()* width;
    float randY = FMath::FRand() * height;
    FVector2f firstPoint(width, height);
    //RandomQueue works like a queue, except that it
    //pops a random element from the queue instead of
    //the element at the head of the queue
    Points.Push(firstPoint);
    ProcessList.Push(firstPoint);
    //generate the first point randomly
    //and updates 

    //generate other points from points in queue.
    while (!ProcessList.IsEmpty())
    {
        FVector2f point = ProcessList.Pop();

        for (int i = 0; i < new_points_count; i++)
        {
            FVector2f newPoint = generateRandomPointAround(point, min_dist);
            //check that the point is in the image region
            //and no points exists in the point's neighbourhood
            if (inRectangle(newPoint) and
                not inNeighbourhood(grid, newPoint, min_dist,
                    cellSize))
            {
                //update containers
                processList.push(newPoint);
                samplePoints.push(newPoint);
                grid[imageToGrid(newPoint, cellSize)] = newPoint;
            }
        }
    }
    return samplePoints;
}

FVector2f UPoissonSampler::generateRandomPointAround(FVector2f point, float mindist)
{ //non-uniform, favours points closer to the inner ring, leads to denser packings
    float r1 = FMath::FRand(); //random point between 0 and 1
    float r2 = FMath::FRand();
    //random radius between mindist and 2 * mindist
    float radius = mindist * (r1 + 1);
    //random angle
    float angle = 2 * PI * r2;
    //the new point is generated around the point (x, y)
    float newX = point.X + radius * cos(angle);
    float newY = point.Y + radius * sin(angle);


    return FVector2f(newX, newY);
}