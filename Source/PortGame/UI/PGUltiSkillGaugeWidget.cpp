// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGUltiSkillGaugeWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"


UPGUltiSkillGaugeWidget::UPGUltiSkillGaugeWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	CurrentUltiSkillGauge = -1.0f;
	PreviousUltiPercent = -1.0f;
}

void UPGUltiSkillGaugeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
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

	CheckUltiSkillActive();
	
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

	CheckUltiSkillActive();
}

void UPGUltiSkillGaugeWidget::CheckUltiSkillActive()
{
	if (Image_Active)
	{
		if (CurrentUltiSkillGauge >= MaxUltiSkillGauge)
			Image_Active->SetVisibility(ESlateVisibility::Visible);
		else
			Image_Active->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPGUltiSkillGaugeWidget::SmoothingGauge(float deltatime)
{
	float CurrentultiGaugePercent = CurrentUltiSkillGauge / MaxUltiSkillGauge;
	float newultiskillGaugePercent = FMath::FInterpTo(PreviousUltiPercent, CurrentultiGaugePercent, deltatime, 3.0f);
	ProgressBar_UltiSkillGauge->SetPercent(newultiskillGaugePercent);
	PreviousUltiPercent = newultiskillGaugePercent;

}
