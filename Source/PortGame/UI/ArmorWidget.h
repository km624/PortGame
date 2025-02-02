// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArmorWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UArmorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UArmorWidget(const FObjectInitializer& ObjectInitializer);
public:
	void SetupArmor(uint8 MaxArmor);

	void UpdateArmor(uint8 CurrentArmor);

	void ShowTextName(bool bisShow);

	void ShowArmorImage(bool bisShow);

	void ShowExecutionKey(bool bSuperGroggy);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox>HorizontalBox_Armor;

	UPROPERTY()
	TObjectPtr<class UTexture2D> ArmorImageAsset;

	UPROPERTY()
	TArray<TObjectPtr<class UImage>> ArmorImages;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr < class UTextBlock> TextBlock_Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr < class UOverlay> Overlay_Execution;
	
	
};
