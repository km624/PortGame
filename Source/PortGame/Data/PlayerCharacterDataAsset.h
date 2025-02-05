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

	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("PlayerData"), GetFName());
	}

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FName Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UltiSkill")
	TObjectPtr<class ULevelSequence> LevelSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution")
	TObjectPtr<class ULevelSequence> ExecutionLevelSequence;
};
