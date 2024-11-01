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

	//몽타주 엔드 델리게이트 형식
	void ComboEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	
	//노티파이 인터페이스
	//virtual void ComboSectionEnd();

	int32 CurrentCombo = 0;

	

	FTimerHandle ComboTimerHandle;

	//발동 타이머 이전에 입력 들어왔나 체크
	bool HasNextComboCommand = false;


};
