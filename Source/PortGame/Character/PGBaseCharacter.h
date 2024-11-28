// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "Interface/ComboCheckInterface.h"
#include "Interface/PGSetHpbarCharacterInterface.h"
#include "Interface/PlayerAttackInterface.h"
#include "Struct/PGCharacterStat.h"
#include "PGBaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerCharacterType :uint8
{
	BlueArchive = 0,
	Nikke,
	ETC
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnbIsAimDelegate, bool /*bIsaim*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnbIshootDelegate, bool /*bIsShoot*/);
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnbIsReloadDelegate, bool /*bIsShoot*/);
DECLARE_MULTICAST_DELEGATE(FOnbIsReloadDelegate);

UCLASS()
class PORTGAME_API APGBaseCharacter : public ACharacter , public IPGSetHpbarCharacterInterface
	, public IPlayerAttackInterface
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterType" ,Meta = (AllowPrivateAccess = "true"))
	EPlayerCharacterType CharacterType;

	//���� ���ؼ�

public:
	FORCEINLINE bool GetCurrentIsAiming() const { return bIsAim; }

	FORCEINLINE bool GetCurrentIsShooting() const { return bIsShoot; }

	FORCEINLINE bool GetCurrentIsReloading() const { return bIsReload; }

	FORCEINLINE void SetbIsReload(bool CurrentReolad) { bIsReload = CurrentReolad; }
	
	float ReturnAimOffset();

	FORCEINLINE FOnbIshootDelegate& GetbIshootDelegate() { return OnbIsShoot; }

	FORCEINLINE FOnbIsReloadDelegate& GetbIsReloadDelegate() { return OnbIsReload; }

	FORCEINLINE FVector GetAimLocation() {return AimLocation;}

	
	
protected:
	UPROPERTY()
	uint8 bIsAim : 1;

	UPROPERTY()
	uint8 bIsShoot : 1;

	UPROPERTY()
	uint8 bIsReload : 1;

	UPROPERTY()
	float AimOffset;

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

	void HitMontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	UFUNCTION()
	void SetDead();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> DeadMontage;

	


};
