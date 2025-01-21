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
