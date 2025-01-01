// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PGBaseCharacter.h"
#include "Interface/PGAICharacterInterface.h"
#include "PGAIBaseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API APGAIBaseCharacter : public APGBaseCharacter ,public IPGAICharacterInterface
{
	GENERATED_BODY()

public:
	APGAIBaseCharacter();

protected:
	virtual void BeginPlay() override;

	//AI 
protected:

	virtual float GetPatrolRadius() override;

	virtual float GetAIDetectRange() override;

	virtual float GetAIAttackRange(float targetDistance, APawn* pawn) override;

	virtual void AttackByAI() override;

	virtual void ShotByAI() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;

	virtual void NotifyComboEnd() override;

	virtual float AITurnSpeed() override;

	virtual bool CheckShotterType() override;

	virtual void SetTimerShooterAttack(float shottime) override;

	virtual bool CheckTargetDead(APawn* Target) override;
	
protected:
	FTimerHandle ShotCompeteTimerHandle;




protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PatrolRadius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectRange = 1000.0f;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurnSpeed = 10.0f;



	FAICharacterAttackFinished OnAttackFinished;

public:
	FORCEINLINE APawn* GetTargetPawn() { return TargetPawn; }

protected:
	TObjectPtr<APawn> TargetPawn;

	//죽었을때 오버라이드
protected:
	virtual void SetDead(int8 teamid) override;
	
};
