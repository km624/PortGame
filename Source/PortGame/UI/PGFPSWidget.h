// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGFPSWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGFPSWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGFPSWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	UPROPERTY(meta =(BindWidget))
	TObjectPtr < class UTextBlock> TextBlock_FPScount;

	
	
};
