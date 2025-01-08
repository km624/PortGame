// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGMainWidget(const FObjectInitializer& ObjectInitializer);

public:
	void SetCharacterWidget(const TArray<class UPlayerCharacterDataAsset*>& characterDatas);

protected:
	UFUNCTION()
	void SelectButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGSelectWidget> BP_SelectWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton>Button_SelectCharacter;
	
};
