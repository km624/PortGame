// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGDashWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGDashWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGDashWidget(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	void SetUpWidget(float dashtime);

	UFUNCTION()
	void OnDash();

	
protected:
	void UpdateDashGuage();

protected:

	float PreviousDashPercent;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_DashGauge;


	UPROPERTY()
	float Dashtime;
	//14°­ ÇöÀç hp
	UPROPERTY()
	float CurrentDashtime;

	FTimerHandle DashtimeTimerHandle;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_FadeOut;


	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_FadeIn;

protected:
	FTimerHandle VisibleHandle;

	UPROPERTY()
	float VisibleTime = 2.0f;

	
};
