// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ComboAttackCheckInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UComboAttackCheckInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTGAME_API IComboAttackCheckInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ComboCheckStart() = 0;

	virtual void ComboCheckEnd() = 0;




};
