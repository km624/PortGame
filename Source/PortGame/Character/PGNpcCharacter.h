// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PGBaseCharacter.h"
#include "Interface/PGNPCCharacterInterface.h"
#include "Interface/NPCParryCheckInterface.h"
#include "PGNpcCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API APGNpcCharacter : public APGBaseCharacter ,public IPGNPCCharacterInterface , public INPCParryCheckInterface
{
	GENERATED_BODY()
	
public:
	APGNpcCharacter();

	virtual void BeginPlay() override;

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void NPCAttackHitStop(float time) override;

	virtual void SetDead() override;
protected:
	FTimerHandle NPCHitStoptimerHandle;

	float currentSlowtime;

protected:

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

	//패리 시스템
protected:
	virtual void OnParryStart(float time) override;

	bool RandomParry();

	virtual void OnParryEnd() override;

	virtual bool GetBisParry() const override;
	
	void NAParryStart();

	void NAParryUpdateScale(float time);
	
	void NAParryUpdateEnd();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Parry")
	uint8 bIsParry : 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Parry", meta =(ClampMin = "0.0", ClampMax = "100.0"))
	float ParryPercent = 30.0f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> ParryNiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Parry")
	TObjectPtr<class UNiagaraSystem> NAParryEffect;

	FTimerHandle NAScaleTimerHandle;

	float ElapsedTime = 0.0f;


};
