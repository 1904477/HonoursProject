// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameState.h"


ACustomGameState::ACustomGameState()
{
	HasGameStarted = false;
	IsGunCollected = false;
	IsEnvironmentScanned = false;
	AreARPlanesDisplayed = true;
	Health = 100;
	SessionModeSelected = VirtualObstacles;
	ReloadNumber = 4;
	AmmoNumber = 10;
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

void ACustomGameState::SetHealth(int b)
{
	Health = b;
}

int ACustomGameState::GetHealth()
{
	return Health;
}

void ACustomGameState::SetAmmo(int b)
{
	AmmoNumber = b;
}

int ACustomGameState::GetAmmo()
{
	return AmmoNumber;
}

void ACustomGameState::SetAmmoReload(int b)
{
	ReloadNumber = b;
}

int ACustomGameState::GetAmmoReload()
{
	return ReloadNumber;
}

void ACustomGameState::SetScore(int b)
{
	Score = b;
}

int ACustomGameState::GetScore()
{
	return Score;
}

bool ACustomGameState::GetIsIsIsOneEnemyAlive()
{
	return IsOneEnemyAlive;
}

void ACustomGameState::SetIsIsIsOneEnemyAlive(bool b)
{
	IsOneEnemyAlive = b;
}

bool ACustomGameState::GetAreARPlanesDisplayed()
{
	return AreARPlanesDisplayed;
}
