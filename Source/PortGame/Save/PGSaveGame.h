// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PGSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPGSaveGame();


public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<FString, int32> CharacterLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class UPlayerCharacterDataAsset>> SelectPlayerDatas;
};
