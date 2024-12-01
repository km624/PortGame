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

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	

public:

	void SetUpWaidget(const FPGCharacterStat& Stat, const FPGCharacterStat& ModifierStat, float NewMaxHitGauge);

	UFUNCTION()
	void UpdateHpBar(float NewCurrentHp);

	void UpdateHitGaugeBar(float NewHitGauge);

protected:
	void SmoothingHp(float deltatime);

	void SmoothingHitGauge(float deltatime);

protected:

float PreviousHpPercent;

float PreviousHitGaugePercent;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_HpBar;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar>ProgressBar_HitGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Hp;

	UPROPERTY()
	float MaxHp;

	//14°­ ÇöÀç hp
	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHitGauge;

	UPROPERTY()
	float CurrentHitGauge;

};
