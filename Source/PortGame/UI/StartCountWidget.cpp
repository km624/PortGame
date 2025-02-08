// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StartCountWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void UStartCountWidget::UpdateTextCount(int32 count)
{
	
	if (count == 0)
	{
		TextBlock_Start->SetText(FText::FromString(TEXT("Start")));
		PlayAnimation(StartAnimation);
	}
	else
	{
		FText text = FText::FromString(FString::Printf(TEXT("%d"), count));
		TextBlock_Start->SetText(text);
		PlayAnimation(UpdateTextAnimation);
	}
	
	
}
