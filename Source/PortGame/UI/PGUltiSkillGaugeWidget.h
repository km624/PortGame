// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGUltiSkillGaugeWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGUltiSkillGaugeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGUltiSkillGaugeWidget(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetUpWidget(float Maxultiskillgauge);

	UFUNCTION()
	void UpdateUltiSkillGauge(float Newgauge);

	void CheckUltiSkillActive();

protected:
	void SmoothingGauge(float deltatime);

protected:

	float PreviousUltiPercent;


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_UltiSkillGauge;

	UPROPERTY( meta = (BindWidget))
	TObjectPtr<class UImage> Image_Active; 
	UPROPERTY()
	float MaxUltiSkillGauge=100.0f;

	//14°­ ÇöÀç hp
	UPROPERTY()
	float CurrentUltiSkillGauge;


};
