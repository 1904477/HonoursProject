// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameState.h"


ACustomGameState::ACustomGameState()
{
	HasGameStarted = false;
	IsEnvironmentScanned = false;
	AreARPlanesDisplayed = true;
	SessionModeSelected = VirtualObstacles;
}


void ACustomGameState::SetAreARPlanesDisplayed(bool value)
{
	AreARPlanesDisplayed = value;
}

bool ACustomGameState::GetHasGameStarted()
{
	return HasGameStarted;
}

void ACustomGameState::SetHasGameStarted(bool b)
{
	HasGameStarted = b;
}

bool ACustomGameState::GetIsEnvironmentScanned()
{
	return IsEnvironmentScanned;
}

void ACustomGameState::SetIsEnvironmentScanned(bool b)
{
	IsEnvironmentScanned = b;
}

bool ACustomGameState::GetIsIsGunCollected()
{
	return IsGunCollected;
}

void ACustomGameState::SetIsIsGunCollected(bool b)
{
	IsGunCollected = b;
}

bool ACustomGameState::GetAreARPlanesDisplayed()
{
	return AreARPlanesDisplayed;
}
