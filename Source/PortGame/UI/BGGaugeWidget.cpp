// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BGGaugeWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "UI/BGGaugeProgressWidget.h"
#include "PortGame/PortGame.h"

UBGGaugeWidget::UBGGaugeWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UBGGaugeProgressWidget> progressbar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/BP_BGGaugeProgress.BP_BGGaugeProgress_C'"));
	if (progressbar.Class)
	{
		BGGaugeProgressClass = progressbar.Class;
	}
	
}

void UBGGaugeWidget::SetGaugeProgressBar(int32 progresscount, float currentGauge)
{
	if (!BGGaugeProgressClass)return;

	
	for (int32 i = 0; i < progresscount; i++)
	{
		UBGGaugeProgressWidget* BGProgressWidget = CreateWidget<UBGGaugeProgressWidget>(GetOwningPlayer(), BGGaugeProgressClass);
		
		AllBGGaugeProgress.Add(BGProgressWidget);


		HorizontalBox_AllGuage->AddChildToHorizontalBox(BGProgressWidget);
	
	}
	UpdateBGGauge(currentGauge);
}

void UBGGaugeWidget::UpdateBGGauge(float CurrentGauge)
{
	//SLOG(TEXT("CurrentGaugeWidget : %f"), CurrentGauge);
	float Totalgauge = CurrentGauge;
	for (int32 i = 0; i < AllBGGaugeProgress.Num(); i++)
	{
		float UpdateGauge=0.0f;
		if (0.0f < Totalgauge)
		{
			if (Totalgauge >= MaxGauge)
			{
				UpdateGauge = MaxGauge;
			}
			else
			{
				UpdateGauge = Totalgauge;
			}
			AllBGGaugeProgress[i]->UpdateBGGaugeProgress(UpdateGauge);
			Totalgauge -= MaxGauge;
		}
		else
		{
			if (Totalgauge <= 0.0f)
			{
				UpdateGauge = 0.0f;
			}
			AllBGGaugeProgress[i]->UpdateBGGaugeProgress(UpdateGauge);
			
		}

	}
}
