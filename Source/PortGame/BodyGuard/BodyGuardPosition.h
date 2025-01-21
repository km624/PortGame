// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BodyGuard/BodyGuardBase.h"
#include "BodyGuardPosition.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBodyGuardPosition : public UBodyGuardBase
{
	GENERATED_BODY()



protected:
	uint8 bIsLined : 1;
	
};
