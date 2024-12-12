// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/PGCharacterStat.h"
#include "PGHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGHudWidget(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION()
	void SetUpWidget(const FPGCharacterStat& BaseStat, const FPGCharacterStat& ModifierStat);
	UFUNCTION()
	void SetupUltiSkillWidget(const float currentGague);

	UFUNCTION()
	void SetupSkillWidget(float cooltime);
	//ABHPBar의 Update와 동일한 함수
	UFUNCTION()
	void UpdateHpBar(float NewCurrentHp);
	UFUNCTION()
	void UpdateHitGaugeBar(float NewHitGauge);
	UFUNCTION()
	void UpdateUltiSkillGaugeBar(float newUltiGauge);
	UFUNCTION()
	void StartSkillCoolTime(bool start);

	UFUNCTION()
	void CorssHairEnable(bool bIsaim);

protected:
	virtual void NativeConstruct() override;



	//초기화 함수에서 초기화하기
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGHPBarWidget> BP_HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_CrossHair;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGUltiSkillGaugeWidget> BP_UltiSkillGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USkillWidget> BP_SkillWidget;
	//UPROPERTY()
	//TObjectPtr<class UABCharacterStatWidget> CharacterStat;
	
};
