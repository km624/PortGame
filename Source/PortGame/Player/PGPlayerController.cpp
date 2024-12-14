// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PGPlayerController.h"

#include "UI/PGHudWidget.h"

#include "Character/PGPlayerCharacter.h"

#include "Component/PGWidgetComponent.h"

#include "PortGame/PortGame.h"

APGPlayerController::APGPlayerController()
{
	
}

void APGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

}

void APGPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	APGPlayerCharacter* currentplayer = Cast<APGPlayerCharacter>(aPawn);
	PlayerCharacters.Add(currentplayer);

	SetViewTarget(GetPawn());

	
	if (currentplayer)
	{
		currentplayer->HiddenWidget();
		currentplayer->HudWidgetAddviewport();
	}
}

void APGPlayerController::PlayCameraShake(TSubclassOf<class UCameraShakeBase> camerashake)
{
	ClientStartCameraShake(camerashake);
}

void APGPlayerController::ChangedCharacterPossess(int8 playernum)
{
	APGPlayerCharacter* changeplayer = nullptr;

	if(PlayerCharacters.IsValidIndex(playernum))
		changeplayer = PlayerCharacters[playernum];
	else
	{
		SLOG(TEXT("NoCharacter!!"));
		return;
	}

	if (changeplayer == GetPawn())
	{
		SLOG(TEXT("SameCharacter"));
		return;
	}
	APGPlayerCharacter* currentplayer = Cast<APGPlayerCharacter>(GetPawn());
	if (currentplayer)
	{
		currentplayer->RemoveHudWidget();
	}
	UnPossess();
	Possess(changeplayer);
}

