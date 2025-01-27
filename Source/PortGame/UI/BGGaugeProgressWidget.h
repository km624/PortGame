// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGGaugeProgressWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBGGaugeProgressWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UBGGaugeProgressWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateBGGaugeProgress(float currentpercent);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_gauge;

protected:
	float MaxPercent = 100.0f;

	float CurrentPercent=0.0f;
	
};
