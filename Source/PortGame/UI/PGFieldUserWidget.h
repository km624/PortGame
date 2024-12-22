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

	void ChangeColor(uint8 teamid);
	UFUNCTION()
	void UpdateFieldGauge(float Newgauge);

	void SetLockImage(bool lock);


protected:
	void SmoothingGauge(float deltatime);

protected:

	float PreviousGaugePercent;

	
	uint8 Teamid;


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_FieldGauge;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_Lock;

	
	UPROPERTY()
	float MaxFieldGauge;

	//14°­ ÇöÀç hp
	UPROPERTY()
	float CurrentFieldGauge;

	
};
