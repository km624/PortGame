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

	void CharacterButtonEnable();


protected:
	UFUNCTION()
	void TurnBackToMainWidget();

	UFUNCTION()
	void OnSelectComplete();
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
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_BlockButton;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Complete;
	

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_BackMain;

public:
	void SetUpAllStatWidget();

	void UpdateStatWidget(int8 num,class UPlayerCharacterDataAsset* palyerdata, int32 level);

	void UpdateWeaponStatWidget(int8 num, class UWeaponData* weapondata);

	void HiddenStatWidget();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGStatWidget> BP_FirstStatWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGStatWidget> BP_SecondStatWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGStatWidget> BP_ThirdStatWidget;

};
