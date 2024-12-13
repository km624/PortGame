// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGUltiSkillGaugeWidget.h"
#include "Components/ProgressBar.h"

UPGUltiSkillGaugeWidget::UPGUltiSkillGaugeWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	CurrentUltiSkillGauge = -1.0f;
	PreviousUltiPercent = -1.0f;
}

void UPGUltiSkillGaugeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UPGUltiSkillGaugeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SmoothingGauge(InDeltaTime);
}

void UPGUltiSkillGaugeWidget::SetUpWidget(float currentUlti)
{
	
	CurrentUltiSkillGauge = currentUlti;
	
	if (ProgressBar_UltiSkillGauge)
	{
		ProgressBar_UltiSkillGauge->SetPercent(CurrentUltiSkillGauge / MaxUltiSkillGauge);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("progress miss"));
	}
	PreviousUltiPercent = CurrentUltiSkillGauge / MaxUltiSkillGauge;
	
	this->SetVisibility(ESlateVisibility::Visible);
}

void UPGUltiSkillGaugeWidget::UpdateUltiSkillGauge(float Newgauge)
{
	//이전 백분율
	PreviousUltiPercent = CurrentUltiSkillGauge / MaxUltiSkillGauge;

	if (ProgressBar_UltiSkillGauge)
	{

		CurrentUltiSkillGauge = Newgauge;


	}

}

void UPGUltiSkillGaugeWidget::SmoothingGauge(float deltatime)
{
	float CurrentultiGaugePercent = CurrentUltiSkillGauge / MaxUltiSkillGauge;
	float newultiskillGaugePercent = FMath::FInterpTo(PreviousUltiPercent, CurrentultiGaugePercent, deltatime, 3.0f);
	ProgressBar_UltiSkillGauge->SetPercent(newultiskillGaugePercent);
	PreviousUltiPercent = newultiskillGaugePercent;

}
