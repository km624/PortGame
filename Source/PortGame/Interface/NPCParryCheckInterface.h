// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NPCParryCheckInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNPCParryCheckInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTGAME_API INPCParryCheckInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void OnParryStart(float time) = 0;
	virtual void OnParryEnd() = 0;
	virtual void NPCAttackHitStop(float time) = 0;

	virtual bool GetBisParry() const;
};
