// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "Interface/ComboCheckInterface.h"
#include "PGBaseCharacter.generated.h"

UCLASS()
class PORTGAME_API APGBaseCharacter : public ACharacter 
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APGBaseCharacter();

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UComboData> ComboData;

	void ComboStart();

	void ComboBegin();

	void ComboCheckTimer();

	void ComboCheck();

	//��Ÿ�� ���� ��������Ʈ ����
	void ComboEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	
	//��Ƽ���� �������̽�
	//virtual void ComboSectionEnd();

	int32 CurrentCombo = 0;

	

	FTimerHandle ComboTimerHandle;

	//�ߵ� Ÿ�̸� ������ �Է� ���Գ� üũ
	bool HasNextComboCommand = false;


};
