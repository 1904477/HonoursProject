// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameState.h"


ACustomGameState::ACustomGameState()
{
	HasGameStarted = false;
	AreARPlanesDisplayed = true;
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

bool ACustomGameState::GetAreARPlanesDisplayed()
{
	return AreARPlanesDisplayed;
}
