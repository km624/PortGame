// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Character/PGBaseCharacter.h"
//#include "Interface/PGNPCCharacterInterface.h"
#include "Character/PGAIBaseCharacter.h"
#include "Interface/NPCParryCheckInterface.h"
#include "PGNpcCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API APGNpcCharacter : public APGAIBaseCharacter , public INPCParryCheckInterface
{
	GENERATED_BODY()
	
public:
	APGNpcCharacter();

	virtual void BeginPlay() override;

	void ChangeNpcColor();

protected:
	//팀 색상을 동적 변경 위함
	UMaterialInstanceDynamic* DynamicMaterial;

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void NPCAttackHitStop(float time) override;

	virtual void SetDead(int8 teamid) override;

	void ReturnCharacterToPool();
protected:
	
	FTimerHandle NPCHitStoptimerHandle;
	
	float currentSlowtime;

	FTimerHandle DeadHiddentimerHandle;

protected:
	virtual void PlayHitMontage() override;


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
	float ParryPercent = 40.0f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> ParryNiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Parry")
	TObjectPtr<class UNiagaraSystem> NAParryEffect;

	FTimerHandle NAScaleTimerHandle;

	float ElapsedTime = 0.0f;


};
