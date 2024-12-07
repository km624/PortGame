// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/CharacterEnumData.h"
#include "Interface/PGSetHpbarCharacterInterface.h"
#include "Interface/PlayerAttackInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Struct/PGCharacterStat.h"
#include "PGBaseCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnbIsAimDelegate, bool /*bIsaim*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnbIshootDelegate, bool /*bIsShoot*/);

DECLARE_MULTICAST_DELEGATE(FOnbIsReloadDelegate);

UCLASS()
class PORTGAME_API APGBaseCharacter : public ACharacter , public IPGSetHpbarCharacterInterface
	,public IPlayerAttackInterface ,public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APGBaseCharacter();

protected:
	virtual void BeginPlay() override;

protected:
	void AttackToComponent();

protected:
	virtual void ComboCheckStartToComp() override;

	virtual void ComboCheckEndToComp() override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterType" ,Meta = (AllowPrivateAccess = "true"))
	EPlayerCharacterType CharacterType;

	//���� ���ؼ�

public:
	FORCEINLINE void SetPlayerCharacterType(EPlayerCharacterType newType) { CharacterType = newType; }

	FORCEINLINE bool GetCurrentIsAiming() const { return bIsAim; }

	FORCEINLINE bool GetCurrentIsShooting() const { return bIsShoot; }

	FORCEINLINE bool GetCurrentIsReloading() const { return bIsReload; }

	FORCEINLINE void SetbIsReload(bool CurrentReolad) { bIsReload = CurrentReolad; }
	
	//�ִϸ��̼� �ν��Ͻ��� �ʿ�
	//���� ���϶� �޴ϸ��̼�
	float ReturnAimOffset();

	/*FORCEINLINE FOnbIshootDelegate& GetbIshootDelegate() { return OnbIsShoot; }

	FORCEINLINE FOnbIsReloadDelegate& GetbIsReloadDelegate() { return OnbIsReload; }*/

	FORCEINLINE FVector GetAimLocation() {return AimLocation;}

	
	
protected:
	UPROPERTY(VisibleAnywhere)
	uint8 bIsAim : 1;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsShoot : 1;

	UPROPERTY()
	uint8 bIsReload : 1;

	UPROPERTY()
	float AimOffset;

public:

	FOnbIsAimDelegate OnbIsAim;

	FOnbIshootDelegate OnbIsShoot;

	FOnbIsReloadDelegate OnbIsReload;

	UPROPERTY()
	FVector AimLocation;

protected:
	void ReloadToWeapon();


	//���� ���� 
public:
	FPGCharacterStat GetTotalStat();

	void SetUpBaseStat(FName baseStat);

	void SetUpModifierStat(FPGCharacterStat ModiferStat);

	FORCEINLINE bool GetbIsDead() { return bIsDead; }
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPGStatComponent> StatComponent;

	uint8 bIsDead : 1;

	

	//����
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPGAttackComponent> AttackComponent;

	
	
	//���� ����
public:
	void HiddenWidget();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPGWidgetComponent> HpBarWidgetComponent;

	virtual void SetUpHpWidget(class UPGUserWidget* InUserWidget) override;


	//���� 
protected:
	//���� üũ�Լ�
	virtual void AttackHitCheckToComp() override;

	//�𸮾� ��ü���� �ִ°� ��� - ���Ϳ������� ����������
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	//�ǰ� �ִϸ��̼�
protected:
	UFUNCTION()
	void PlayHitMontage();

	//�ڷ� �и���
	virtual void HitGaugeZeroEffect() override;

	void HitMontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

protected:
	//�׷α����
	uint8 bIsGroggy : 1;


	UFUNCTION()
	virtual void SetDead();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> DeadMontage;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MotionWarping, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComponent;


	//�� ����
public:
	FORCEINLINE virtual void SetGenericTeamId(const FGenericTeamId& teamID) override { myteam = teamID; }

	FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const override { return myteam; }


protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	uint8 TeamId;

	FGenericTeamId myteam;


	//���̾ư��� ����Ʈ ��� (����)
public:
	void StartNiagaraEffect(class UNiagaraSystem* niagara, FVector TargetLocation);

	UFUNCTION()
	void OnNiagaraSystemFinished(class UNiagaraComponent* FinishedComponent);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> BaseNiagaraComponent;

	//Skill �Է� ������Ʈ��
protected:
	void SkillToComponent();

	

};
