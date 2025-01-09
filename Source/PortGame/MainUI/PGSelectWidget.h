// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/CharacterEnumData.h"
#include "PGSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGSelectWidget(const FObjectInitializer& ObjectInitializer);
public:
	void SetUpPlayerButton(const TArray<class UPlayerCharacterDataAsset*>& characterDatas);

	void SetUpSwordData(const TArray<class UWeaponData*>& sworddatas);
	
	void SetUpGunData(const TArray<class UWeaponData*>& gunddatas);

	void ShowSelectWeaponWidget(EPlayerCharacterType characterType);

protected:
	TSubclassOf<class UPGPlayerButtonWidget> PlayerButtonClass;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> VerticalBox_BlueArchive;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> VerticalBox_Nikke;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> VerticalBox_ETC;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGSelectWeaponWidget> BP_SelectWeapon_Sword;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGSelectWeaponWidget> BP_SelectWeapon_Gun;
	

};
