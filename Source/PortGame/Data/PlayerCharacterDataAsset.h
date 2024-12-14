// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/BaseCharacterDataAsset.h"
#include "PlayerCharacterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPlayerCharacterDataAsset : public UBaseCharacterDataAsset
{
	GENERATED_BODY()

public:
	UPlayerCharacterDataAsset();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UltiSkill")
	TObjectPtr<class ULevelSequence> LevelSequence;
};
