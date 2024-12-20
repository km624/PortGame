// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FieldManagerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFieldManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTGAME_API IFieldManagerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual class UFieldManager* GetFieldManager() const = 0;
};
