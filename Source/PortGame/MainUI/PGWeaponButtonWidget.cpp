// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI/PGWeaponButtonWidget.h"
#include "Player/mainPlayerController.h"
#include "Data/GunWeaponData.h"
#include "Data/SwordWeaponData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "PortGame/PortGame.h"
#include "MainUI/PGSelectWeaponWidget.h"

UPGWeaponButtonWidget::UPGWeaponButtonWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

void UPGWeaponButtonWidget::SetWeaponData(UWeaponData* weaponData, UPGSelectWeaponWidget* selectwidget)
{
	PlayerController = Cast<AmainPlayerController>(GetOwningPlayer());
	SelectWeaponWidget = selectwidget;
	WeaponData = weaponData;
	FText WeaponName = FText::FromString(WeaponData->WeaponMesh->GetName());

	TextBlock_WeaponName->SetText(WeaponName);

	//SetButtonStyle(false);

	Button_SelectWeapon->OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);

}

void UPGWeaponButtonWidget::OnButtonClicked()
{

	PlayerController->SetSelectWeaponrData(WeaponData);
	SelectWeaponWidget->SetVisibility(ESlateVisibility::Hidden);
	/*bool selected = PlayerController->SetSelectCharcterData(CharacterData);

	SetButtonStyle(selected);*/
}
