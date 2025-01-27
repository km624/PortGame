// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BodyGuard/BodyGuardBase.h"
#include "BodyGuardAroundPosition.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBodyGuardAroundPosition : public UBodyGuardBase
{
	GENERATED_BODY()

public:
	UBodyGuardAroundPosition();

public:
	virtual FVector CalculatePawnPostion(AActor* player, int32 index, int32 arrayConut);

protected:
	FVector DefacultCalculateRotator(FVector originalVector, float angleDegrees);
	
};
