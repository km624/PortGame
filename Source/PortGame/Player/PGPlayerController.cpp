// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PGPlayerController.h"

#include "UI/PGHudWidget.h"

#include "Character/PGPlayerCharacter.h"

#include "Component/PGWidgetComponent.h"

#include "PortGame/PortGame.h"

#include "AI/PGAIController.h"

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
	//PlayerCharacters.Add(currentplayer);
	SetViewTarget(GetPawn());
	currentplayer->SetCharacterInputData(EControlData::Base);
	SLOG(TEXT("Possess :%s"), *aPawn->GetActorNameOrLabel());
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


	if (changeplayer->GetbIsDead())
	{
		SLOG(TEXT("CharacterDead"));
		return;
	}
	APGPlayerCharacter* currentplayer = Cast<APGPlayerCharacter>(GetPawn());
	
	UnPossess();

	ChangeCharacterController(changeplayer, currentplayer);

	//UnPossess();
	Possess(changeplayer);
}

void APGPlayerController::SpawnCharacterAdd(APGPlayerCharacter* character,APGAIController* aicontroller)
{
	if (character&& aicontroller)
	{

		PlayerCharacters.Add(character);
		AIPlayerControllers.Add(aicontroller);
		SLOG(TEXT("%s : add"), *character->GetActorNameOrLabel());
	}
	
}

void APGPlayerController::ChangeCharacterController(APGPlayerCharacter* newcharacter, APGPlayerCharacter* oldcharacter)
{

	if (oldcharacter)
	{
		if (PlayerCharacters.Contains(oldcharacter))
		{
			//전에 hud 지우기
			oldcharacter->RemoveHudWidget();

			//AI 컨트롤러 possess
			APGAIController* oldaicontroller = AIPlayerControllers[PlayerCharacters.IndexOfByKey(oldcharacter)];
			if (oldaicontroller)
			{
				oldaicontroller->SetAttackAIData();
				oldaicontroller->Possess(oldcharacter);
			}
		}

	}

	if (newcharacter)
	{
		if (PlayerCharacters.Contains(newcharacter))
		{

			newcharacter->SetbIsAim(false);
			//ai 컨트롤러 초기화 진행
			APGAIController* newaicontroller = AIPlayerControllers[PlayerCharacters.IndexOfByKey(newcharacter)];

			if (newaicontroller)
			{
				newaicontroller->AIIsDead();
			}
		}
	}
}

