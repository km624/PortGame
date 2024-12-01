// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTGAME_API IAIControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void StopTree() = 0;
	virtual void StartTree() = 0;
};
