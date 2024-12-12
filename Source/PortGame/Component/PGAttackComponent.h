// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PGAttackComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNextComboDelegate, bool /*HasNextComboCommand*/);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTGAME_API UPGAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPGAttackComponent();

protected:

	virtual void InitializeComponent() override;
	
	virtual void BeginPlay() override;

	

public:
	void SetWeaponClass();

	void SetUpWeapon();

	void AttackToWeapon();

	FORCEINLINE class UWeaponData* GetWeaponData() { return WeaponData; }


public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWeaponData> WeaponData;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<class AWeapon> Weapon;


public:
	void ComboCheckStart();

	void CombocheckEnd();

	
protected:
	FOnNextComboDelegate OnNextCombo;

	uint8 ComboTiming : 1;

	uint8 HasNextComboCommand : 1;

public:
	void AttackHitCheck();

	//�÷��̾ �ش�
	
protected:
	//���� ��¦ ���߱� (�÷��̾ �ش�)
	void AttackHitStop(float time, TSubclassOf<class UCameraShakeBase> camerashake);

	//���� ����Ʈ
	void AttackEffect(AActor* target,FVector targetLocation);
protected:
	FTimerHandle HitStoptimerHandle;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CamreShake)
	TSubclassOf<class UCameraShakeBase> AttackCameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CamreShake)
	TSubclassOf<class UCameraShakeBase>	ParrayCameraShakeClass;

public:
	UFUNCTION()
	FORCEINLINE bool GetbIsGodMode() { return bIsGodMode; }

	FORCEINLINE void SetbIsGodMode(bool isgod) { bIsGodMode = isgod; }
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = palyerState)
	uint8 bIsGodMode : 1;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = palyerState)
	int32 ParryCount = 0;*/
	

	//ĳ���� ��ų
public:
	void SetSkill();

	void SkillAttack();

	void WeaponHide(bool visible);


public:
	FORCEINLINE class USkillBase* GetSkill() { return Skill; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<class USkillBase> SkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<class USkillBase> Skill;

	//�ñر�
public:
	void SetUltiSkill();

	void UltiSkillAttack();

	void FirstUltiSkillEffect();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UltiSkill")
	TSubclassOf<class UUltiSkill> UltiSkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UltiSkill")
	TObjectPtr<class UUltiSkill> UltiSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float FirstSkillSlowTime = 3.0f;

		

};
