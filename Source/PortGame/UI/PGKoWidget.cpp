// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGKoWidget.h"
#include "Components/TextBlock.h"



void UPGKoWidget::UpdateKOCount(int32 count)
{
	FText countText = FText::FromString(FString::Printf(TEXT("%d"), count));
	TextBlock_Count->SetText(countText);
}
