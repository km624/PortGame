// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGGunStatProgressBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGGunStatProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGGunStatProgressBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetupGunStat(float maxstat, FString statname);


	void UpdateGunStat(float newstat);

protected:
	void SmoothingGunStat(float deltatime);


protected:
	UPROPERTY()
	float MaxStat;

	//14°­ ÇöÀç hp
	UPROPERTY()
	float CurrentGunStat;

	float PreviousGunPercent;


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_GunStat;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_StatName;



	
};
