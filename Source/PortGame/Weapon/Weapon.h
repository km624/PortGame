// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PORTGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	AWeapon();

	virtual void OnInitializeWeapon(class  APGBaseCharacter* BaseCharacter);

	virtual void Attack();

public:
	FORCEINLINE FName GetWeaponFname(){ return WeaponSocket; }

protected:
	
	virtual void BeginPlay() override;


	//기본 변수
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class APGBaseCharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> WeaponaSkeletalComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<USkeletalMesh> WeaponMesh;

	FName WeaponSocket;

	

	//기본 공격 ( 콤보)
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combo)
	TObjectPtr<class UAnimMontage> ComboMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UComboData> ComboData;

	int32 CurrentCombo = 0;

	FTimerHandle ComboTimerHandle;

	FTimerHandle ComboTimerdelayHandle;


	bool HasNextComboCommand = false;


protected:

	void ComboStart();

	void ComboBegin();

	void ComboCheckTimer();

	void ComboCheck();

	//몽타주 엔드 델리게이트 형식
	void ComboEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

};
