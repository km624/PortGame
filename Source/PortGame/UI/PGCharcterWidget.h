// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct/PGCharacterStat.h"
#include "PGCharcterWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGCharcterWidget : public UUserWidget
{
	GENERATED_BODY()



public:
	void SetStyle(class UTexture2D* charcterimage);

	void CheckIsMine(bool bismine);

	void SetChangeCoolTime(float cooltime);
	UFUNCTION()
	void SetUphpbarWidget(const FPGCharacterStat& Stat, const FPGCharacterStat& ModifierStat);

	UFUNCTION()
	void UpdateHpBar(float NewCurrentHp);

	UFUNCTION()
	void UpdateHitGaugeBar(float NewHitGauge);


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGHPBarWidget> BP_HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_Character;

	TObjectPtr<class UTexture2D> CharacerImage;
	FSlateBrush BackgroundBrush;
	FSlateBrush FillBrush;

public:

	void StartChangeCooltime();

protected:
	void UpdateChangeCooltime();

	UPROPERTY()
	float Changecooltime;
	
	UPROPERTY()
	float Currentcooltime;

	FTimerHandle ChangeCooltimeTimerHandle;
	
};
