// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PGAICharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPGAICharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
DECLARE_DELEGATE(FAICharacterAttackFinished);
class PORTGAME_API IPGAICharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetPatrolRadius() = 0;

	virtual float  GetAIDetectRange() = 0;

	virtual float GetAIAttackRange(float targetDistance, APawn* pawn) = 0;

	virtual void AttackByAI() = 0;

	virtual void ShotByAI() = 0;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) = 0;

	virtual void NotifyComboEnd() = 0;

	virtual float AITurnSpeed() = 0;

	virtual bool CheckShotterType() = 0;

	virtual void SetTimerShooterAttack(float shottime) = 0;

	virtual bool CheckTargetDead(APawn* Target) = 0;

	virtual void NotVisibleAttack(APawn* Target) = 0;
	
};
