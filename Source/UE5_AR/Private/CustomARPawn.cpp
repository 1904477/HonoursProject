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
	CapsuleComponent->SetupAttachment(CameraComponent);

	CapsuleComponent->InitCapsuleSize(30,90);
	CapsuleComponent->SetCollisionProfileName("OverlapAll");
}

// Called when the game starts or when spawned
void ACustomARPawn::BeginPlay()
{
	Super::BeginPlay();
	auto Temp = GetWorld()->GetAuthGameMode();
	ACustomGameMode* GM = Cast<ACustomGameMode>(Temp);

	auto Temp1 = GetWorld()->GetGameState();
	GS = Cast<ACustomGameState>(Temp1);
	GameManager = GM->GameManager;
}

// Called every frame
void ACustomARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	camLocation = camManager->GetCameraLocation();
	if (GS->GetHealth() <= 0)
		GS->SetDidPlayerLose(true);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, CapsuleComponent->GetComponentLocation().ToString());

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
		if (GS->GetIsEnvironmentScanned())
		{
			FHitResult HitResult;
			if (GS->GetIsIsGunCollected() == false)
			{
				// Perform a hitTest, get the return values as hitTesTResult
				if (!WorldHitTest(HitResult))
				{
					return;
				}
				// Get object of actor hit.
				UClass* hitActorClass = UGameplayStatics::GetObjectClass(HitResult.GetActor());
				// if we've hit a target.
				if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, AGunPickup::StaticClass()))
				{
					Gun = Cast<AGunPickup>(HitResult.GetActor());
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::SanitizeFloat((Gun->GetActorLocation() - GetActorLocation()).Length()));

					if ((Gun->GetActorLocation() - GetActorLocation()).Length() < 200)
					{
						GS->SetIsIsGunCollected(true);
						Gun->BoxComponent->SetCollisionProfileName("NoCollision");
					}
				}
			}
			else
			{
				if (Gun->inShootingAnimation)
				{
					// Perform a hitTest, get the return values as hitTesTResult
					if (!WorldHitTest(HitResult))
					{
						return;
					}
					// Get object of actor hit.
					UClass* hitActorClass = UGameplayStatics::GetObjectClass(HitResult.GetActor());
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, HitResult.GetComponent()->GetName());

					// if we've hit a target.
					if ((UKismetMathLibrary::ClassIsChildOf(hitActorClass, ASpawnedEnemy::StaticClass()) && GS->GetAmmo() > 0))
					{
						ASpawnedEnemy* HitEnemy = Cast<ASpawnedEnemy>(HitResult.GetActor());
						HitEnemy->Health -= 20;
					}
				}
			}
		}
	}
}

void ACustomARPawn::OnScreenRelease(const ETouchIndex::Type FingerIndex, const FVector ScreenPos)
{
}

bool ACustomARPawn::WorldHitTest(FHitResult& fHit)
{
	// Get player controller
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	// Perform deprojection taking 2d clicked area and generating reference in 3d world-space.
	// construct trace vector (from point clicked to 1000.0 units beyond in same direction)
	FVector traceEndVector = camManager->GetActorForwardVector() * 2000.0f;
	traceEndVector = camLocation + traceEndVector;
	// perform line trace (Raycast)
	bool traceSuccess = GetWorld()->LineTraceSingleByChannel(fHit, camLocation, traceEndVector,
		ECollisionChannel::ECC_WorldDynamic);
	// return if the operation was successful
	return traceSuccess;
}