// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PGIconWidget.h"
#include "Data/CharacterEnumData.h"
#include "PGPlayerIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGPlayerIconWidget : public UPGIconWidget
{
	GENERATED_BODY()

public:
	
	UPGPlayerIconWidget(const FObjectInitializer& ObjectInitializer);

public:

	void SetUpPlayerIconImage(EPlayerCharacterType charactertype ,bool bismine, int8 mynum, class UPGMiniMapWidget* minimap);

	void ChangePlayerIcon(bool bisminimap);

protected:
	UFUNCTION()
	void ClickSelectIndex();
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Character;

	UPROPERTY()
	TMap<EPlayerCharacterType, TObjectPtr<class UTexture2D>> CharactertypeImages;
	
	UPROPERTY()
	int8 myindex;
	

	UPROPERTY()
	TObjectPtr<class UPGMiniMapWidget> MiniMap;
};
