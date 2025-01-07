// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Struct/PGCharacterStat.h"
#include "GameInstance/PGGameInstanceInterface.h"
#include "PGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGGameInstance : public UGameInstance,public IPGGameInstanceInterface
{
	GENERATED_BODY()

public:
	UPGGameInstance();

	
	virtual FPGCharacterStat GetStat(FName rarity) override; 

private:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, FPGCharacterStat> AllStat;

	//TArray<>SelectPlayerData;
	
};
