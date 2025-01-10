// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGPlayerButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGPlayerButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGPlayerButtonWidget(const FObjectInitializer& ObjectInitializer);

public:
	void SetCharacterData(class UPlayerCharacterDataAsset* characterData);

protected:
	void SetButtonStyle(bool select);

	UFUNCTION()
	void OnButtonClicked();

	UFUNCTION()
	void OnButtonHovered();

protected:
	UPROPERTY()
	TObjectPtr<class UPlayerCharacterDataAsset> CharacterData;
	UPROPERTY()
	TObjectPtr<class AmainPlayerController> PlayerController;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_SelectCharacter;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_CharacterName;

protected:
	uint8 bIsSelected : 1;
	
};
