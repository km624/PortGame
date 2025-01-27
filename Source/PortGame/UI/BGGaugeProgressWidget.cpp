// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BGGaugeProgressWidget.h"
#include "Components/ProgressBar.h"


UBGGaugeProgressWidget::UBGGaugeProgressWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{

}

void UBGGaugeProgressWidget::UpdateBGGaugeProgress(float currentpercent)
{
	
	if (currentpercent >= 100.0f)
	{
		currentpercent = 100.0f;
	}

	ProgressBar_gauge->SetPercent(currentpercent / MaxPercent);
}
