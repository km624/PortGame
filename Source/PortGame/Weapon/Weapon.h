// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Struct/PGCharacterStat.h"
#include "Weapon.generated.h"

UCLASS()
class PORTGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	AWeapon();

	virtual void OnInitializeWeapon(class  APGBaseCharacter* BaseCharacter, class UWeaponData* weaponData);

	virtual void Attack();

	

public:
	FORCEINLINE FName GetWeaponFname(){ return WeaponSocket; }

protected:
	
	virtual void BeginPlay() override;


	//�⺻ ����
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class APGBaseCharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> WeaponStaticComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<UStaticMesh> WeaponMesh;

	FName WeaponSocket;

public:

	UPROPERTY(EditAnywhere, Category = Stat)
	FPGCharacterStat ModifierStat;

	

	//�⺻ ���� ( �޺�)
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UComboData> ComboData;

	int32 CurrentCombo = 0;

	float ComboPlayTime;

	FTimerHandle ComboTimerHandle;

	FTimerHandle ComboTimerdelayHandle;


	bool HasNextComboCommand = false;

public:
	UFUNCTION()
	void SetHasNextCombo(bool nextcombo);

protected:

	void ComboStart();

	void ComboBegin();

	void ComboCheck();

	//��Ÿ�� ���� ��������Ʈ ����
	void ComboEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	
	FOnMontageEnded EndDelegate;
	FOnMontageEnded* CurrentMontageEndDelegate;
};
