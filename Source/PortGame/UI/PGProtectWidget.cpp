// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGProtectWidget.h"
#include "Components/TextBlock.h"
#include "UI/PGProtectButtonWidget.h"

UPGProtectWidget::UPGProtectWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	MaxCount = 0;

	CurrentCount = 0;
}

void UPGProtectWidget::SetupMaxProtectCount(int32 max)
{
	
	TextBlock_MaxCount->SetText(FText::AsNumber(max));

}

void UPGProtectWidget::UpdateprotectCount(int32 currentnum)
{
	TextBlock_CurrentCount->SetText(FText::AsNumber(currentnum));
}

void UPGProtectWidget::SetUpOptionButton(AActor* playercharacter, int32 optionSize)
{
	TArray<TObjectPtr<UPGProtectButtonWidget>> OptionsButton= {
	   BP_ProtectOptionButton_1,
	   BP_ProtectOptionButton_2,
	   BP_ProtectOptionButton_3,
	   BP_ProtectOptionButton_4
	};

	for (int32 i = 0; i < optionSize; i++)
	{
		OptionsButton[i]->SetupProtectButton(playercharacter,i+1);
	}
}
