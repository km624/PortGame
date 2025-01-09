// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI/PGMainWidget.h"
#include "MainUI/PGSelectWidget.h"
#include "Data/PlayerCharacterDataAsset.h"
#include "Components/Button.h"
#include "PortGame/PortGame.h"
#include "Data/WeaponData.h"

UPGMainWidget::UPGMainWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

void UPGMainWidget::SetCharacterWidget(const TArray<UPlayerCharacterDataAsset*>& characterDatas)
{
	BP_SelectWidget->SetUpPlayerButton(characterDatas);

	Button_SelectCharacter->OnClicked.AddDynamic(this, &ThisClass::SelectButtonClicked);
}

void UPGMainWidget::SetWeaponWidget(const TArray<UWeaponData*>& swordDatas, const TArray<UWeaponData*>& gunDatas)
{
	BP_SelectWidget->SetUpSwordData(swordDatas);
	BP_SelectWidget->SetUpGunData(gunDatas);
}

void UPGMainWidget::ShowWeaponWidget(EPlayerCharacterType characterType)
{
	BP_SelectWidget->ShowSelectWeaponWidget(characterType);
}

void UPGMainWidget::SelectButtonClicked()
{
	SLOG(TEXT("Visiibe"));
	BP_SelectWidget->SetVisibility(ESlateVisibility::Visible);
}
