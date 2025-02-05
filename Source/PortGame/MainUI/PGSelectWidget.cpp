// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI/PGSelectWidget.h"
#include "MainUI/PGPlayerButtonWidget.h"
#include "Data/PlayerCharacterDataAsset.h"
#include "Components/VerticalBox.h"
#include "Data/CharacterEnumData.h"
#include "Components/VerticalBoxSlot.h"
#include "Data/WeaponData.h"
#include "MainUI/PGSelectWeaponWidget.h"
#include "Player/mainPlayerController.h"
#include "Components/Button.h"
#include "MainUI/PGStatWidget.h"
#include "Components/Image.h"

UPGSelectWidget::UPGSelectWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UPGPlayerButtonWidget> playerButton(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/Main/BP_SelectButton.BP_SelectButton_C'"));
	if (playerButton.Class)
	{
		PlayerButtonClass = playerButton.Class;
	}
}

void UPGSelectWidget::SetUpPlayerButton(const TArray<UPlayerCharacterDataAsset*>& characterDatas)
{
	Button_BackMain->OnClicked.AddDynamic(this, &ThisClass::TurnBackToMainWidget);
	Button_Complete->OnClicked.AddDynamic(this, &ThisClass::OnSelectComplete);

	if (characterDatas.Num() > 0 && PlayerButtonClass)
	{
		for (UPlayerCharacterDataAsset* Data : characterDatas)
		{
			UPGPlayerButtonWidget* Button = CreateWidget<UPGPlayerButtonWidget>(GetOwningPlayer(), PlayerButtonClass);
			Button->SetCharacterData(Data);

			UVerticalBoxSlot* VerticalBoxSlot = nullptr;
			if (Data->Charactertype == EPlayerCharacterType::BlueArchive)
			{
				VerticalBoxSlot = VerticalBox_BlueArchive->AddChildToVerticalBox(Button);
			}
			else if (Data->Charactertype == EPlayerCharacterType::Nikke)
			{
				VerticalBoxSlot= VerticalBox_Nikke->AddChildToVerticalBox(Button);
			}
			else
			{
				VerticalBoxSlot= VerticalBox_ETC->AddChildToVerticalBox(Button);
			}
			if (VerticalBoxSlot)
			{
				
				VerticalBoxSlot->SetPadding(FMargin(0.0f, 15.0f, 0.0f, 0.0f));
			}

		}
	}

}

void UPGSelectWidget::SetUpSwordData(const TArray<UWeaponData*>& sworddatas)
{
	BP_SelectWeapon_Sword->SetUpWeapon(sworddatas);
}

void UPGSelectWidget::SetUpGunData(const TArray<UWeaponData*>& gunddatas)
{
	BP_SelectWeapon_Gun->SetUpWeapon(gunddatas);
}

void UPGSelectWidget::ShowSelectWeaponWidget(EPlayerCharacterType characterType)
{
	Image_BlockButton->SetVisibility(ESlateVisibility::Visible);

	if (characterType == EPlayerCharacterType::ETC)
	{
		BP_SelectWeapon_Sword->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BP_SelectWeapon_Gun->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPGSelectWidget::CharacterButtonEnable()
{
	Image_BlockButton->SetVisibility(ESlateVisibility::Collapsed);
}



void UPGSelectWidget::TurnBackToMainWidget()
{
	AmainPlayerController* playerController = Cast<AmainPlayerController>(GetOwningPlayer());

	if (playerController)
	{
		playerController->SetUpMainWidget();
	}
}

void UPGSelectWidget::OnSelectComplete()
{
	AmainPlayerController* playerController = Cast<AmainPlayerController>(GetOwningPlayer());

	if (playerController)
	{
		playerController->SelectComplete();
	}
}

void UPGSelectWidget::SetUpAllStatWidget()
{
	BP_FirstStatWidget->SetUpAllProgressBar();
	BP_SecondStatWidget->SetUpAllProgressBar();
	BP_ThirdStatWidget->SetUpAllProgressBar();
}

void UPGSelectWidget::UpdateStatWidget(int8 num,UPlayerCharacterDataAsset* palyerdata,int32 level)
{

	switch (num)
	{
	case 0:
		BP_FirstStatWidget->SetVisibility(ESlateVisibility::Visible);
		BP_FirstStatWidget->UpdateBaseStat(palyerdata,level);
		break;

	case 1:
		BP_SecondStatWidget->SetVisibility(ESlateVisibility::Visible);
		BP_SecondStatWidget->UpdateBaseStat(palyerdata, level);
		break;

	default:
		BP_ThirdStatWidget->SetVisibility(ESlateVisibility::Visible);
		BP_ThirdStatWidget->UpdateBaseStat(palyerdata, level);
		break;
	}
}

void UPGSelectWidget::UpdateWeaponStatWidget(int8 num, UWeaponData* weapondata)
{
	switch (num)
	{
	case 0:
		BP_FirstStatWidget->UpdateWeaponStat(weapondata);
		break;

	case 1:
		BP_SecondStatWidget->UpdateWeaponStat(weapondata);
		break;

	default:
		BP_ThirdStatWidget->UpdateWeaponStat(weapondata);
		break;
	}
}

void UPGSelectWidget::HiddenStatWidget()
{
	BP_FirstStatWidget->SetVisibility(ESlateVisibility::Hidden);
	BP_SecondStatWidget->SetVisibility(ESlateVisibility::Hidden);
	BP_ThirdStatWidget->SetVisibility(ESlateVisibility::Hidden);
}
