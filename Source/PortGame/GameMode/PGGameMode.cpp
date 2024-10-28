// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PGGameMode.h"
#include "Character/PGPlayerCharacter.h"
#include "Player/PGPlayerController.h"


APGGameMode::APGGameMode()
{
	DefaultPawnClass = APGPlayerCharacter::StaticClass();

	PlayerControllerClass = APGPlayerController::StaticClass();
}
