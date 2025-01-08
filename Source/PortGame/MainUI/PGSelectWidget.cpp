// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI/PGSelectWidget.h"
#include "MainUI/PGPlayerButtonWidget.h"
#include "Data/PlayerCharacterDataAsset.h"
#include "Components/VerticalBox.h"
#include "Data/CharacterEnumData.h"
#include "Components/VerticalBoxSlot.h"

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
