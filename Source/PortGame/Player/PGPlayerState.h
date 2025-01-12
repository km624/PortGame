// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PGPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API APGPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	APGPlayerState();

public:
	void GameClear();

	void GameOver();
};
