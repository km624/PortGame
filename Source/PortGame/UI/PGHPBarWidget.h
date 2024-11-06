// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PGUserWidget.h"
#include "Struct/PGCharacterStat.h"
#include "PGHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGHPBarWidget : public UPGUserWidget
{
	GENERATED_BODY()

public:
	UPGHPBarWidget(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void NativeConstruct() override;

public:

	void SetUpWaidget(const FPGCharacterStat& Stat);

	UFUNCTION()
	void UpdateHpBar(float NewCurrentHp);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Hp;

	UPROPERTY()
	float MaxHp;

	//14�� ���� hp
	UPROPERTY()
	float CurrentHp;

};
