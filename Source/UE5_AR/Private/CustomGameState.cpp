// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameState.h"


ACustomGameState::ACustomGameState() :
	Score(50)
{
	IsPlaceableSelected = false;
	IsPlaceableSpawned = false;
	AreARPlanesDisplayed = true;
}

int32 ACustomGameState::GetScore()
{
	return Score;
}

void ACustomGameState::SetScore(const int32 NewScore)
{
	Score = NewScore;
}

void ACustomGameState::SetPlaceableSelected(bool value)
{
	IsPlaceableSelected = value;
}

void ACustomGameState::SetAreARPlanesDisplayed(bool value)
{
	AreARPlanesDisplayed = value;
}

bool ACustomGameState::GetPlaceableSelected()
{
	return IsPlaceableSelected;
}

bool ACustomGameState::GetAreARPlanesDisplayed()
{
	return AreARPlanesDisplayed;
}
