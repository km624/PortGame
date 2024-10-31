// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PGGameMode.h"
#include "Character/PGPlayerCharacter.h"
#include "Player/PGPlayerController.h"


APGGameMode::APGGameMode()
{
	static::ConstructorHelpers::FClassFinder<APGPlayerCharacter> PlayerCharacter
	(TEXT("/Script/Engine.Blueprint'/Game/PortGame/Blueprint/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if(PlayerCharacter.Class)
		DefaultPawnClass = PlayerCharacter.Class;
	//DefaultPawnClass = APGPlayerCharacter::StaticClass();
	PlayerControllerClass = APGPlayerController::StaticClass();
}
