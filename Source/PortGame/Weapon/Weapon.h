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

	virtual void WeaponAttachment(class  APGBaseCharacter* BaseCharacter);

	virtual void Attack();

protected:
	
	virtual void BeginPlay() override;


	//�⺻ ����
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class APGBaseCharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> WeaponaSkeletalComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<USkeletalMesh> WeaponMesh;


	//�⺻ ���� ( �޺�)
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

	//��Ÿ�� ���� ��������Ʈ ����
	void ComboEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

};
