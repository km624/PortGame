// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGStatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetUpAllProgressBar();

	void UpdateBaseStat(class UPlayerCharacterDataAsset* characterData);

	void UpdateWeaponStat(class UWeaponData* weapondata);


protected:
	float MaxHp = 3000.0f;
	float MaxAttack = 300.0f;
	float MaxAttackRange = 300.0f;
	float MaxAttackSpeed = 2.0f;
	float MaxHitGauge = 300.0f;

	float MaxAmmo = 100.0f;
	float MaxReload = 2.0f;
	float MaxShootInterval = 2.0f;
	float MaxTraceDistance = 3000.0f;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGStatProgressBarWidget> BP_StatProgressBar_HP;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGStatProgressBarWidget> BP_StatProgressBar_Attack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGStatProgressBarWidget> BP_StatProgressBar_Range;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGStatProgressBarWidget> BP_StatProgressBar_AttackSpeed;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGStatProgressBarWidget> BP_StatProgressBar_HitGauge;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> VerticalBox_Gun;
	

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGGunStatProgressBarWidget> BP_GunProgressBar_MaxAmmo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGGunStatProgressBarWidget> BP_GunProgressBar_Reload;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGGunStatProgressBarWidget> BP_GunProgressBar_ShootInterval;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGGunStatProgressBarWidget> BP_GunProgressBar_TraceDistanced;
	
};
