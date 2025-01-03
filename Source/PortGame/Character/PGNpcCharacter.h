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

	virtual void EnableCharacter() override;

	virtual void Tick(float deltatime) override;

	void ChangeNpcColor();

protected:
	void SetPhysicsSetting();

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
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReturnPoolTime = 4.0f;

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
	float ParryPercent = 30.0f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> ParryNiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Parry")
	TObjectPtr<class UNiagaraSystem> NAParryEffect;

	FTimerHandle NAScaleTimerHandle;

	float ElapsedTime = 0.0f;
	
public:
	FORCEINLINE bool GetbIsAttackField() const { return bIsAttackField; }

	FORCEINLINE void SetbIsAttackField(bool bisattacckfield) { bIsAttackField = bisattacckfield; }

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	uint8 bIsAttackField : 1;

public:
	void ForceReturnObjectPool();

protected:
	void CheckCharacterRender();

	void NotRenderCharacter();

	void OnRenderCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Optimization")
	uint8 bIsRendered : 1;


};
