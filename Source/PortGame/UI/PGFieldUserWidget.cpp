// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGFieldUserWidget.h"
#include "Components/ProgressBar.h"
#include "PortGame/PortGame.h"
#include "Components/Image.h"

UPGFieldUserWidget::UPGFieldUserWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	PreviousGaugePercent = -1.0f;
	MaxFieldGauge = -1.0f;
	CurrentFieldGauge = -1.0f;
}

void UPGFieldUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UPGFieldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	SmoothingGauge(InDeltaTime);
}

void UPGFieldUserWidget::SetUpWidget(uint8 teamid, float maxgauge, float currentgauge)
{
	
	MaxFieldGauge = maxgauge;
	CurrentFieldGauge = currentgauge;
	ChangeColor(teamid);
	if (ProgressBar_FieldGauge)
	{
		ProgressBar_FieldGauge->SetPercent(CurrentFieldGauge / MaxFieldGauge);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("progress miss"));
	}

	PreviousGaugePercent = CurrentFieldGauge / MaxFieldGauge;
	
	

	this->SetVisibility(ESlateVisibility::Visible);
}

void UPGFieldUserWidget::ChangeColor(uint8 teamid)
{
	FLinearColor linearcolor;
	if (teamid == 1)
	{
		linearcolor = FLinearColor(0.0f, 0.5f, 1.0f, 1.0f);
	}
	else
	{
		linearcolor = FLinearColor(1.0f, 0.02f, 0.02f, 1.0f);
	}

	ProgressBar_FieldGauge->SetFillColorAndOpacity(linearcolor);
}

void UPGFieldUserWidget::UpdateFieldGauge(float Newgauge)
{
	//이전 백분율
	PreviousGaugePercent = CurrentFieldGauge / MaxFieldGauge;

	if (ProgressBar_FieldGauge)
	{

		CurrentFieldGauge = Newgauge;

	}

}

void UPGFieldUserWidget::SetLockImage(bool lock)
{
	if (lock)
	{
		Image_Lock->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image_Lock->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPGFieldUserWidget::SmoothingGauge(float deltatime)
{
	float CurrentfieldPercent = CurrentFieldGauge / MaxFieldGauge;
	float newfieldGaugePercent = FMath::FInterpTo(PreviousGaugePercent, CurrentfieldPercent, deltatime, 2.0f);
	ProgressBar_FieldGauge->SetPercent(newfieldGaugePercent);
	PreviousGaugePercent = newfieldGaugePercent;

}
