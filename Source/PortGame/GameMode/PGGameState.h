// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PGGameState.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API APGGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	APGGameState();

public:
	void GameClear();

	void GameOver();
	
};
