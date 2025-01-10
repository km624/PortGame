// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGWeaponButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGWeaponButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGWeaponButtonWidget(const FObjectInitializer& ObjectInitializer);

public:
	void SetWeaponData(class UWeaponData* weaponData,class UPGSelectWeaponWidget* selectwidget);

protected:
	//void SetButtonStyle(bool select);s

	UFUNCTION()
	void OnButtonClicked();

	UFUNCTION()
	void OnButtonHovered();

protected:
	UPROPERTY()
	TObjectPtr<class UWeaponData> WeaponData;
	UPROPERTY()
	TObjectPtr<class AmainPlayerController> PlayerController;
	UPROPERTY()
	TObjectPtr<class UPGSelectWeaponWidget> SelectWeaponWidget;

	

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_SelectWeapon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_WeaponName;
	
};
