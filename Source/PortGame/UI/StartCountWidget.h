// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartCountWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UStartCountWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void UpdateTextCount(int32 count);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Start;


	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> UpdateTextAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> StartAnimation;

	
};
