// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectPoolingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObjectPoolingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTGAME_API IObjectPoolingInterface
{
	GENERATED_BODY()

	
public:

	virtual class UObjectPoolManager* GetObjectPoolManager() const = 0;
};
