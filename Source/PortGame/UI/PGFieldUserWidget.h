// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGFieldUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGFieldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGFieldUserWidget(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	void SetUpWidget(uint8 teamid,float maxgauge,float currentgauge);


	UFUNCTION()
	void UpdateFieldGauge(float Newgauge);


protected:
	void SmoothingGauge(float deltatime);

protected:

	float PreviousGaugePercent;

	
	uint8 Teamid;


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_FieldGauge;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_Active;
	
	UPROPERTY()
	float MaxFieldGauge;

	//14°­ ÇöÀç hp
	UPROPERTY()
	float CurrentFieelGauge;

	
};
