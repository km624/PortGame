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

	//공격 살짝 멈추기 (플레이어만 해당)
protected:
	void AttackHitStop();

protected:
	FTimerHandle HitStoptimerHandle;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CamreShake)
	TSubclassOf<class UCameraShakeBase> AttackCameraShakeClass;


	

		
};
