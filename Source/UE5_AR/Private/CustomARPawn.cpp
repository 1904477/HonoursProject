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

	//Camera component attached to scene component.
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SceneComponent);

	//Collision capsule attached to camera, important to detect collision as player moves in AR, otherwise capsule is static in starting position.
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(CameraComponent);

	//Collision capsule initialised.
	CapsuleComponent->InitCapsuleSize(30,90);
	CapsuleComponent->SetCollisionProfileName("OverlapAll");

}

// Called when the game starts or when spawned.
void ACustomARPawn::BeginPlay()
{
	Super::BeginPlay();

	//Get Gamemode.
	auto Temp = GetWorld()->GetAuthGameMode();
	ACustomGameMode* GM = Cast<ACustomGameMode>(Temp);

	//Get GameState.
	auto Temp1 = GetWorld()->GetGameState();
	GS = Cast<ACustomGameState>(Temp1);

	//Set Gamemanager reference.
	GameManager = GM->GameManager;
}

// Called every frame
void ACustomARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Set Camera manager.
	camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	//Set camera location.
	camLocation = camManager->GetCameraLocation();

	if (GS->GetHealth() <= 0)
		GS->SetDidPlayerLose(true);
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
	if (GameManager)	//If game manager exists.
	{
		if (GS->GetIsEnvironmentScanned())
		{
			FHitResult HitResult;
			if (GS->GetIsIsGunCollected() == false)
			{
				// Perform a hitTest, get the return values as hitTestResult.
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

					//If player is relatively close to the gun, the gun can be collected.
					if ((Gun->GetActorLocation() - camLocation).Length() < 200)		
					{
						GS->SetIsIsGunCollected(true);
						Gun->BoxComponent->SetCollisionProfileName("NoCollision");		//Disable gun collision for shooting
					}
				}
			}
		}
	}
}

void ACustomARPawn::OnScreenRelease(const ETouchIndex::Type FingerIndex, const FVector ScreenPos)
{
}

void ACustomARPawn::Shoot()
{
	FHitResult HitResult;
	// Perform a hitTest, get the return values as hitTesTResult
	if (!WorldHitTest(HitResult))
	{
		return;
	}
	// Get object of actor hit.
	UClass* hitActorClass = UGameplayStatics::GetObjectClass(HitResult.GetActor());
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, HitResult.GetComponent()->GetName());

	// if we've hit a target.
	if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, ASpawnedEnemy::StaticClass()))
	{
		ASpawnedEnemy* HitEnemy = Cast<ASpawnedEnemy>(HitResult.GetActor());

		//Reduce enemy health.
		HitEnemy->Health -= 50;		
		
	}
}

bool ACustomARPawn::WorldHitTest(FHitResult& fHit)
{
	// Get player controller
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	// Perform deprojection taking 2d clicked area and generating reference in 3d world-space.
	// construct trace vector (from point clicked to 1000.0 units beyond in same direction)
	FVector traceEndVector = camManager->GetActorForwardVector() * 2000.0f;

	//If gun exists and is collected, linetrace from gun position for shooting, otherwise linetrace from camera location.
	if(Gun)
	{
		traceEndVector = Gun->GetActorLocation() + traceEndVector;
		// perform line trace (Raycast)
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(this->GetOwner());
		queryParams.AddIgnoredActor(Gun);

		bool traceSuccess = GetWorld()->LineTraceSingleByChannel(fHit, Gun->GetActorLocation(), traceEndVector,
			ECollisionChannel::ECC_WorldDynamic, queryParams);
		
		return traceSuccess;

	}
	else
	{
		traceEndVector = camLocation + traceEndVector;
		// perform line trace (Raycast)
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(this->GetOwner());
		bool traceSuccess = GetWorld()->LineTraceSingleByChannel(fHit, camLocation, traceEndVector,
			ECollisionChannel::ECC_WorldDynamic, queryParams);

		return traceSuccess;
	}
}

