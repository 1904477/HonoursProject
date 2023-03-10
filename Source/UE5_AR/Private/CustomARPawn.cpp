// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomARPawn.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ARBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "CustomPickup.h"
#include "GunPickup.h"
#include "SpawnedEnemy.h"
#include "Camera/CameraComponent.h"


// Sets default values
ACustomARPawn::ACustomARPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SceneComponent);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(SceneComponent);

	CapsuleComponent->InitCapsuleSize(30,90);
	CapsuleComponent->SetCollisionProfileName("OverlapAll");
}

// Called when the game starts or when spawned
void ACustomARPawn::BeginPlay()
{
	Super::BeginPlay();
	auto Temp = GetWorld()->GetAuthGameMode();
	ACustomGameMode* GM = Cast<ACustomGameMode>(Temp);
	GameManager = GM->GameManager;
}

// Called every frame
void ACustomARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	camLocation = camManager->GetCameraLocation();

}

// Called to bind functionality to input
void ACustomARPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind various player inputs to functions
	// There are a few types - BindTouch, BindAxis, and BindEvent.  
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACustomARPawn::OnScreenTouch);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACustomARPawn::OnScreenRelease);
}

void ACustomARPawn::OnScreenTouch(const ETouchIndex::Type FingerIndex, const FVector ScreenPos)
{
	if (GameManager)
	{
		// Perform a hitTest, get the return values as hitTesTResult
		if (!WorldHitTest(FVector2D(ScreenPos), fHitRes))
		{
			// Get object of actor hit.
			UClass* hitActorClass = UGameplayStatics::GetObjectClass(fHitRes.GetActor());
			// if we've hit a target.
			if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, ACustomPickup::StaticClass()))
				UKismetSystemLibrary::PrintString(this, "Cube clicked!", true, true, FLinearColor(0, 0.66, 1, 1), 2);
		}
	}
}

void ACustomARPawn::OnScreenRelease(const ETouchIndex::Type FingerIndex, const FVector ScreenPos)
{

}

bool ACustomARPawn::WorldHitTest(FVector2D screenTouch, FHitResult fHit)
{
	// Get player controller
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	// Perform deprojection taking 2d clicked area and generating reference in 3d world-space.
	FVector worldPosition;
	FVector worldDirection; // Unit Vector
	bool deprojectionSuccess = UGameplayStatics::DeprojectScreenToWorld(playerController, screenTouch, /*out*/
		worldPosition, /*out*/ worldDirection);

	// construct trace vector (from point clicked to 1000.0 units beyond in same direction)
	FVector traceEndVector = worldDirection * 1000.0;
	traceEndVector = worldPosition + traceEndVector;
	// perform line trace (Raycast)
	bool traceSuccess = GetWorld()->LineTraceSingleByChannel(fHit, worldPosition, traceEndVector,
		ECollisionChannel::ECC_WorldDynamic);
	// return if the operation was successful
	return traceSuccess;
}