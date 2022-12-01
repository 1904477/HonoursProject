// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceablePlayer.h"
#include "GameManager.h"
#include "CustomGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"

APlaceablePlayer::APlaceablePlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	

	// auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	// //StaticMeshComponent->SetupAttachment(SceneComponent);
	//
	// static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("Material'/Engine/EngineMaterials/PPM_DefaultCalibrationColor.PPM_DefaultCalibrationColor'"));
	// //static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("Material'/Engine/EngineMaterials/PPM_DefaultCalibrationColor.PPM_DefaultCalibrationColor'"));
	// if (FoundMaterial.Succeeded())
	// {
	// 	StoredMaterial = FoundMaterial.Object;
	// 	//GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Red, TEXT("Player material correctly found"));
	//
	// }
	//DynamicMaterialInst = UMaterialInstanceDynamic::Create(StoredMaterial, StaticMeshComponent);

	
}
void APlaceablePlayer::BeginPlay()
{
	Super::BeginPlay();
	auto Temp = GetWorld()->GetAuthGameMode();
	GM = Cast<ACustomGameMode>(Temp);
	GameManager = GM->GameManager;
	GetMesh()->BodyInstance.bLockYRotation = true;
	GetMesh()->BodyInstance.bLockXRotation = true;
	//SetState(Idle);
	//SetActorRotation(FRotator(0,0,0));
	//StaticMeshComponent->SetMaterial(0, DynamicMaterialInst);
	AIController = Cast<AAIController>(GetController());
	if(!AIController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, TEXT("AIController not found"));
	}
	
}

void APlaceablePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsPlayerIdle==false)
	{
		MovePlayer();
	}
	
}

// Called to bind functionality to input
void APlaceablePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind various player inputs to functions
	// There are a few types - BindTouch, BindAxis, and BindEvent.  
}

void APlaceablePlayer::MovePlayer()
{
	if(AIController)
	{
		FVector ActualPosition = GetActorLocation();
		//FVector MoveDirection =  - ActualPosition;
		//FRotator NewRot = (GameManager->MoveLocation - ActualPosition).Rotation();
		//SetActorRotation(NewRot);
		if (DoesPlayerWalk == true)		//If player walks 
		{
			//AIController->MoveTo(GameManager->MoveLocation );
			AIController->MoveToLocation(GameManager->MoveLocation,-1,false, true );
		}
		else if (DoesPlayerJump == true)
		{
			//AIController->MoveTo(GameManager->MoveLocation );
			//AIController->MoveToLocation(GameManager->MoveLocation,-1,false,true );
			FVector move = FMath::VInterpTo(ActualPosition, FVector(GameManager->MoveLocation.X, GameManager->MoveLocation.Y, GameManager->MoveLocation.Z+50.f), GetWorld()->GetDeltaSeconds(), 2);
			FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(GameManager->ActualPosition, GameManager->MoveLocation);
			SetActorRotation(PlayerRot);
			SetActorLocation(move);
		}
		DistanceToTarget(GameManager->MoveLocation - GetActorLocation());
	}
}

void APlaceablePlayer::DistanceToTarget(FVector DestinationRange)
{
	if(DoesPlayerJump)
	{
		if(abs(DestinationRange.X)<30.f&&abs(DestinationRange.Y)<30.f&& abs(DestinationRange.Z)<20.f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, TEXT("Player Stopped"));
			Controller->StopMovement();
			IsPlayerIdle = true;
			DoesPlayerWalk = false;
			DoesPlayerJump = false;
			CanPlayerJump = false;
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
	else if(DoesPlayerWalk)
	{
		if(abs(DestinationRange.X)<20.f&&abs(DestinationRange.Y)<20.f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, TEXT("Player Stopped"));
			Controller->StopMovement();
			IsPlayerIdle = true;
			DoesPlayerWalk = false;
			DoesPlayerJump = false;
			CanPlayerJump = false;
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
}















//void APlaceablePlayer::SetState(State NewState)
//{
//	CurrentState = &NewState;  // change state
//}
//FVector move = FMath::VInterpTo(ActualPosition, GameManager->MoveLocation, GetWorld()->GetDeltaSeconds(), 4);
//FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(GameManager->ActualPosition, GameManager->MoveLocation);
//SetActorRotation(PlayerRot);
//SetActorLocation(move);