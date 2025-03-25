// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLevel/PGGameLevelScriptActor.h"
#include "Field/PGField.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/GameplayStatics.h"
#include "PortGame/PortGame.h"
#include "Field/FieldManager.h"
#include "Field/ObjectPoolManager.h"
#include "GameFramework/PlayerStart.h"
#include "AI/PGAIController.h"
#include "Player/PGPlayerController.h"

APGGameLevelScriptActor::APGGameLevelScriptActor()
{
	bGameStart = false;
}

UFieldManager* APGGameLevelScriptActor::GetFieldManager() const
{
	return FieldManager;
}

UObjectPoolManager* APGGameLevelScriptActor::GetObjectPoolManager() const
{
	return ObjectPoolManager;
}

void APGGameLevelScriptActor::SetGameStartPlayer(APGPlayerController* playercontroller)
{
	if (playercontroller)
	{
		OnGameStarted.AddUObject(playercontroller, &APGPlayerController::SetGameStart);
		OnCountValueChanged.AddUObject(playercontroller, &APGPlayerController::UpdateStartCount);
		
	}
	
}

void APGGameLevelScriptActor::SetGameStartAI(APGAIController* aicontroller)
{
	if (aicontroller)
	{
		
		OnGameStarted.AddUObject(aicontroller,&APGAIController::SetGameStart);
		
		
	}
	
}

void APGGameLevelScriptActor::SetGameStartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(GameStartTimer,this,&ThisClass::CheckCount,
		 1.0f, true);
}

void APGGameLevelScriptActor::CheckCount()
{
	if (GameStartCount < 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(GameStartTimer);
		bGameStart = true;
		OnGameStarted.Broadcast(bGameStart);
	}
	else
	{
		OnCountValueChanged.Broadcast(GameStartCount);
		GameStartCount--;
	}
}

int32 APGGameLevelScriptActor::GetStartCount()
{
	return GameStartCount;
}

bool APGGameLevelScriptActor::GetbGameStart()
{
	return bGameStart;
}

void APGGameLevelScriptActor::GameEnd()
{
	bGameStart = false;
	OnGameStarted.Broadcast(bGameStart);
}

void APGGameLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	

	ObjectPoolManager = NewObject<UObjectPoolManager>(this);

	if (ObjectPoolManager)
	{
		ObjectPoolManager->InitializePool();

	}
	FieldManager = NewObject<UFieldManager>(this);
	if (FieldManager)
	{
		FieldManager->InitializeFieldManager();
		
	}

	//SetGameStartTimer();
}

