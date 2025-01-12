// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PGGameState.h"
#include "PortGame/PortGame.h"
#include "Player/PGPlayerState.h"

APGGameState::APGGameState()
{
}

void APGGameState::GameClear()
{
	
	if (PlayerArray.Num() > 0)
	{
		for (APlayerState* player : PlayerArray)
		{
			APGPlayerState* playerstate = Cast<APGPlayerState>(player);

			if (playerstate)
			{
				playerstate->GameClear();
			}
		}
	
	}
}

void APGGameState::GameOver()
{
	if (PlayerArray.Num() > 0)
	{
		for (APlayerState* player : PlayerArray)
		{
			APGPlayerState* playerstate = Cast<APGPlayerState>(player);

			if (playerstate)
			{
				playerstate->GameOver();
			}
		}

	}
}
