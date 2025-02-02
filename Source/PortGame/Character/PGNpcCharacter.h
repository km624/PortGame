// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Character/PGBaseCharacter.h"
//#include "Interface/PGNPCCharacterInterface.h"
#include "Character/PGAIBaseCharacter.h"
#include "Interface/NPCParryCheckInterface.h"
#include "Interface/NPCTargetLockInterface.h"
#include "PGNpcCharacter.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitDelegate, bool/*bIshit*/)
UCLASS()
class PORTGAME_API APGNpcCharacter : public APGAIBaseCharacter , public INPCParryCheckInterface ,public INPCTargetLockInterface
{
	GENERATED_BODY()
	
public:
	APGNpcCharacter();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	virtual void SetupCharacterData(class UBaseCharacterDataAsset* characterdata) override;

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

	virtual void SetDead(AActor* DamageCauser) override;

	void CommonNPCDeadLogic(AActor* DamageCauser);

	void ReturnCharacterToPool();

	//위젯 컴포넌트 관련
protected:
	
	UPROPERTY()
	uint8 bIshit : 1;

	FOnHitDelegate OnHited;

	FTimerHandle NPCHitTimer;
	UPROPERTY()
	float WidgetShowTime = 10.0f;

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

	//보류 -  거리 기반 업데이트 비율 설정
	void SetAnimationDistanceFactor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Optimization")
	uint8 bIsRendered : 1;


protected:

	virtual void SetTargeting(bool targeting) override;


protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr < class UWidgetComponent> TargetWidget;

};
