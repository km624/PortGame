// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Struct/PGCharacterStat.h"
#include "PGGameInstanceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPGGameInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTGAME_API IPGGameInstanceInterface
{
	GENERATED_BODY()

	
public:
	virtual FPGCharacterStat GetStat(FName rarity) = 0;

	
};
