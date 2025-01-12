// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGKoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGKoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	

	UFUNCTION()
	void UpdateKOCount(int32 count);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Count;
	
};
