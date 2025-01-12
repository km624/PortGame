// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PGPlayerState.h"
#include "Player/PGPlayerController.h"

APGPlayerState::APGPlayerState()
{
}

void APGPlayerState::GameClear()
{
	APGPlayerController* pgplayercontroller = Cast<APGPlayerController>(GetPlayerController());
	if (pgplayercontroller)
	{
		pgplayercontroller->GameClear();
	}
}

void APGPlayerState::GameOver()
{
	APGPlayerController* pgplayercontroller = Cast<APGPlayerController>(GetPlayerController());
	if (pgplayercontroller)
	{
		pgplayercontroller->GameOver();
	}
}
