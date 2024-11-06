// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "Interface/ComboCheckInterface.h"
#include "Interface/PGSetHpbarCharacterInterface.h"
#include "PGBaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerCharacterType :uint8
{
	BlueArchive = 0,
	Nikke,
	ETC
};


UCLASS()
class PORTGAME_API APGBaseCharacter : public ACharacter , public IPGSetHpbarCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APGBaseCharacter();

	
protected:
	void AttackToComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterType" ,Meta = (AllowPrivateAccess = "true"))
	EPlayerCharacterType CharacterType;

	//���� ������

public:
	FORCEINLINE bool GetCurrentIsAiming() const { return bIsAim; }

	float ReturnAimOffset();

protected:
	UPROPERTY()
	uint8 bIsAim : 1;

	UPROPERTY()
	float AimOffset;

//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =Combo)
//	TObjectPtr<class UAnimMontage> ComboMontage;
//
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = "true"))
//	TObjectPtr<class UComboData> ComboData;
//
//	void ComboStart();
//
//	void ComboBegin();
//
//	void ComboCheckTimer();
//
//	void ComboCheck();
//
//	//��Ÿ�� ���� ��������Ʈ ����
//	void ComboEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
//	
//	//��Ƽ���� �������̽�
//	//virtual void ComboSectionEnd();
//
//	int32 CurrentCombo = 0;
//
//	FTimerHandle ComboTimerHandle;
//
//	FTimerHandle ComboTimerdelayHandle;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combo)
//	float ComoboDelay;
//
//	uint8 ComboOK : 1;
//
//	//�ߵ� Ÿ�̸� ������ �Է� ���Գ� üũ
//	bool HasNextComboCommand = false;

	//���� ���� (Hp)
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPGStatComponent> StatComponent;

	//����
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPGAttackComponent> AttackComponent;

	//���� ����
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPGWidgetComponent> HpBarWidgetComponent;

	virtual void SetUpHpWidget(class UPGUserWidget* InUserWidget) override;
};
