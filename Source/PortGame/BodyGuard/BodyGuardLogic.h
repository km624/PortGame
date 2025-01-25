// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BodyGuard/BodyGuardBase.h"
#include "BodyGuardLogic.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBodyGuardLogic : public UBodyGuardBase
{
	GENERATED_BODY()
	
public:
	UBodyGuardLogic();

public:
	
	virtual float GetBodyGuardSpeed();

};
