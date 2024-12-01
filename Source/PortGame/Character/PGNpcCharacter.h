// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PGBaseCharacter.h"
#include "Interface/PGNPCCharacterInterface.h"
#include "PGNpcCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API APGNpcCharacter : public APGBaseCharacter ,public IPGNPCCharacterInterface
{
	GENERATED_BODY()
	
public:
	APGNpcCharacter();

	virtual void BeginPlay() override;

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual float GetPatrolRadius() override;

	virtual float GetAIDetectRange() override;

	virtual float GetAIAttackRange(float targetDistance, APawn* pawn) override;

	virtual void AttackByAI() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;

	virtual void NotifyComboEnd() override;

	virtual float AITurnSpeed() override;


	

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float PatrolRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurnSpeed;



	FAICharacterAttackFinished OnAttackFinished;

public:
	FORCEINLINE APawn* GetTargetPawn() {return TargetPawn;}

protected:
	TObjectPtr<APawn> TargetPawn;


};
