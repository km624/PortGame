// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI/PGGunStatProgressBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Player/mainPlayerController.h"


UPGGunStatProgressBarWidget::UPGGunStatProgressBarWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	MaxStat = -1.0f;
	CurrentGunStat = -1.0f;
	PreviousGunPercent = -1.0f;
}

void UPGGunStatProgressBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	SmoothingGunStat(InDeltaTime);
}

void UPGGunStatProgressBarWidget::SetupGunStat(float maxstat, FString statname)
{
	MaxStat = maxstat;
	CurrentGunStat = 0.0f;
	
	PreviousGunPercent = 0.0f;

	if (ProgressBar_GunStat)
	{
		ProgressBar_GunStat->SetPercent(CurrentGunStat / MaxStat);

	}

	if (TextBlock_StatName)
	{
		TextBlock_StatName->SetText(FText::FromString(statname));
	}
}

void UPGGunStatProgressBarWidget::UpdateGunStat(float newstat)
{
	PreviousGunPercent = CurrentGunStat / MaxStat;



	if (ProgressBar_GunStat)
	{

		CurrentGunStat = newstat;


	}
}

void UPGGunStatProgressBarWidget::SmoothingGunStat(float deltatime)
{
	float CurrentGunPercent = CurrentGunStat / MaxStat;

	float NewHpPercent = FMath::FInterpTo(PreviousGunPercent, CurrentGunPercent, deltatime, 2.0f);
	ProgressBar_GunStat->SetPercent(NewHpPercent);
	PreviousGunPercent = NewHpPercent;
}
