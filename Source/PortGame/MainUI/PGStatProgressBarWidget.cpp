// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI/PGStatProgressBarWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

UPGStatProgressBarWidget::UPGStatProgressBarWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	
	MaxStat = -1;
	CurrentBaseStat = -1;
	CurrentWeaponStat = -1;

	PreviousBasePercent = -1;

	PreviousWeaponPercent = -1;
}

void UPGStatProgressBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	SmoothingBaseStat(InDeltaTime);
	SmoothingWeaponStat(InDeltaTime);
}

void UPGStatProgressBarWidget::SetupCharacterStat(float maxstat, FString statname)
{
	MaxStat = maxstat;
	CurrentBaseStat = 0.0f; 
	CurrentWeaponStat = 0.0f;
	PreviousBasePercent = 0.0f;

	PreviousWeaponPercent = 0.0f;

	if (ProgressBar_Base)
	{
		ProgressBar_Base->SetPercent(CurrentBaseStat / MaxStat);

	}
	
	if (ProgressBar_Weapon)
	{

		ProgressBar_Weapon->SetPercent(CurrentWeaponStat + CurrentBaseStat / MaxStat);

	}

	if (TextBlock_StatName)
	{
		TextBlock_StatName->SetText(FText::FromString(statname));
	}

}

void UPGStatProgressBarWidget::UpdateBase(float newstat)
{
	
	PreviousBasePercent = CurrentBaseStat / MaxStat;

	

	if (ProgressBar_Base)
	{

		CurrentBaseStat = newstat;
	

	}


}

void UPGStatProgressBarWidget::UpdateWeapon(float newstat)
{
	
	PreviousWeaponPercent = CurrentWeaponStat / MaxStat;



	if (ProgressBar_Weapon)
	{

		CurrentWeaponStat = newstat + CurrentBaseStat;


	}

}

void UPGStatProgressBarWidget::SmoothingBaseStat(float deltatime)
{
	float CurrentBasePercent = CurrentBaseStat / MaxStat;

	float NewHpPercent = FMath::FInterpTo(PreviousBasePercent, CurrentBasePercent, deltatime, 2.0f);
	ProgressBar_Base->SetPercent(NewHpPercent);
	PreviousBasePercent = NewHpPercent;
}

void UPGStatProgressBarWidget::SmoothingWeaponStat(float deltatime)
{
	float CurrentWeaponPercent = CurrentWeaponStat/ MaxStat;

	float NewHpPercent = FMath::FInterpTo(PreviousWeaponPercent, CurrentWeaponPercent, deltatime, 1.0f);
	ProgressBar_Weapon->SetPercent(NewHpPercent);
	PreviousWeaponPercent = NewHpPercent;
}
