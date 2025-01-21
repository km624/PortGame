// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIBodyGuardInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIBodyGuardInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTGAME_API IAIBodyGuardInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool CanPlayerProtect(APawn* pawn) = 0;

	virtual AActor* SetPlayerProtectPawn(APawn* pawn)=0;

	virtual FVector CalculateOffsetPawn(APawn* pawn) = 0;

	//UFUNCTION()
	//virtual void DeletePlayerProtectPawn(APawn* pawn) = 0;
};
