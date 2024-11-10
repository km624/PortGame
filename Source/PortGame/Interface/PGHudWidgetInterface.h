// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PGHudWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPGHudWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTGAME_API IPGHudWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetUpHudWidget(class UPGHudWidget* hudWidget) = 0;
};
