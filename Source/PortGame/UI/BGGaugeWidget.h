// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGGaugeWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBGGaugeWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UBGGaugeWidget(const FObjectInitializer& ObjectInitializer);

	void SetGaugeProgressBar(int32 progresscount,float currentGauge);

	void UpdateBGGauge(float CurrentGauge);


protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TSubclassOf<class UBGGaugeProgressWidget> BGGaugeProgressClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> HorizontalBox_AllGuage;

	UPROPERTY()
	TArray<TObjectPtr< class UBGGaugeProgressWidget>> AllBGGaugeProgress;

protected:
	float MaxGauge = 100.0f;

	
};
