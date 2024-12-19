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
	void SetOwingCharcter(class AActor* owningCharacter);

protected:
	TObjectPtr<AActor> OwningCharacter;

public:
	UFUNCTION()
	void SetUpWidget(const FPGCharacterStat& BaseStat, const FPGCharacterStat& ModifierStat);
	UFUNCTION()
	void SetupUltiSkillWidget(const float currentGague);

	UFUNCTION()
	void SetupSkillWidget(float cooltime);

	UFUNCTION()
	void SetupDashWidget(float dashtime);

	UFUNCTION()
	void SetupGunWidget(int32 maxammo);

	UFUNCTION()
	void SetupReloadWidget(float reloadtime);

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
	void StartDash();

	UFUNCTION()
	void UpdateGunAmmo(int32 Newammo);

	UFUNCTION()
	void StartReload(bool reload, float reloadtime);

	UFUNCTION()
	void SetGunWidgetEnable(bool bIsaim);

	UFUNCTION()
	void ChangeCrosshair(bool bInRange);


public:
	UFUNCTION()
	void SetupFieldGauge(uint8 teamid, float maxgauge, float currentgauge);
	UFUNCTION()
	void UpdateFieldGague(float newGauge);
	UFUNCTION()
	void CollapsedFieldGauge();

protected:
	

	virtual void NativeConstruct() override;


protected:
	bool bIsAim;
	bool bIsReload;

	FLinearColor OriginTint;
	FLinearColor RedTint;


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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGDashWidget> BP_DashWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGGunWidget> BP_GunWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGReloadWidget> BP_ReloadWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGFieldUserWidget> BP_FieldGauge;


	//UPROPERTY()
	//TObjectPtr<class UABCharacterStatWidget> CharacterStat;
	
};
