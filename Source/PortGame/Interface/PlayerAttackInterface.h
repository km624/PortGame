// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTGAME_API IPlayerAttackInterface
{
	GENERATED_BODY()

	
public:
	virtual void AttackHitCheckToComp() = 0;

	virtual void ComboCheckStartToComp() = 0;

	virtual void ComboCheckEndToComp() = 0;

	virtual void HitGaugeZeroEffect() = 0;
};
