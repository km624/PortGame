// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGStatProgressBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGStatProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGStatProgressBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetupCharacterStat(float maxstat ,FString statname);


	void UpdateBase(float newstat, float newlevelstat);

	void UpdateWeapon(float newstat);

protected:
	void SmoothingBaseStat(float deltatime);


	void SmoothingLevelStat(float deltatime);


	void SmoothingWeaponStat(float deltatime);

protected:
	UPROPERTY()
	float MaxStat;

	//14°­ ÇöÀç hp
	UPROPERTY()
	float CurrentBaseStat;

	UPROPERTY()
	float CurrentLevelStat;

	UPROPERTY()
	float CurrentWeaponStat;

	float PreviousBasePercent;

	float PreviousLevelPercent;

	float PreviousWeaponPercent;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_Weapon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_Level;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_Base;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_StatName;


	
};
