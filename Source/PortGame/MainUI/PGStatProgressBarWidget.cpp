// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI/PGStatProgressBarWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

UPGStatProgressBarWidget::UPGStatProgressBarWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	
	MaxStat = -1.0f;
	CurrentBaseStat = -1.0f;
	CurrentWeaponStat = -1.0f;
	CurrentLevelStat = -1.0f;
	PreviousBasePercent = -1.0f;
	PreviousLevelPercent = -1.0f;
	PreviousWeaponPercent = -1.0f;
}

void UPGStatProgressBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	SmoothingBaseStat(InDeltaTime);
	SmoothingLevelStat(InDeltaTime);
	SmoothingWeaponStat(InDeltaTime);
}

void UPGStatProgressBarWidget::SetupCharacterStat(float maxstat, FString statname)
{
	MaxStat = maxstat;
	CurrentBaseStat = 0.0f; 
	CurrentLevelStat = 0.0f;
	CurrentWeaponStat = 0.0f;
	PreviousBasePercent = 0.0f;
	PreviousLevelPercent = 0.0f;
	PreviousWeaponPercent = 0.0f;

	if (ProgressBar_Base)
	{
		ProgressBar_Base->SetPercent(CurrentBaseStat / MaxStat);

	}

	if (ProgressBar_Level)
	{
		ProgressBar_Level->SetPercent(CurrentLevelStat / MaxStat);

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

void UPGStatProgressBarWidget::UpdateBase(float newstat,float newlevelstat)
{
	
	PreviousBasePercent = CurrentBaseStat / MaxStat;
	PreviousLevelPercent = CurrentLevelStat / MaxStat;
	PreviousWeaponPercent = CurrentLevelStat / MaxStat;
	

	if (ProgressBar_Base)
	{

		CurrentBaseStat = newstat;
		CurrentLevelStat = CurrentBaseStat + newlevelstat;
		CurrentWeaponStat = CurrentLevelStat;
	
	}


}

void UPGStatProgressBarWidget::UpdateWeapon(float newstat)
{
	
	PreviousWeaponPercent = CurrentWeaponStat / MaxStat;



	if (ProgressBar_Weapon)
	{

		CurrentWeaponStat = newstat + CurrentLevelStat;


	}

}

void UPGStatProgressBarWidget::SmoothingBaseStat(float deltatime)
{
	float CurrentBasePercent = CurrentBaseStat / MaxStat;

	float NewHpPercent = FMath::FInterpTo(PreviousBasePercent, CurrentBasePercent, deltatime, 2.0f);
	ProgressBar_Base->SetPercent(NewHpPercent);
	PreviousBasePercent = NewHpPercent;
}

void UPGStatProgressBarWidget::SmoothingLevelStat(float deltatime)
{
	float CurrentLevelPercent = CurrentLevelStat / MaxStat;

	float NewHpPercent = FMath::FInterpTo(PreviousLevelPercent, CurrentLevelPercent, deltatime, 2.0f);
	ProgressBar_Level->SetPercent(NewHpPercent);
	PreviousLevelPercent = NewHpPercent;
}

void UPGStatProgressBarWidget::SmoothingWeaponStat(float deltatime)
{
	float CurrentWeaponPercent = CurrentWeaponStat/ MaxStat;

	float NewHpPercent = FMath::FInterpTo(PreviousWeaponPercent, CurrentWeaponPercent, deltatime, 2.0f);
	ProgressBar_Weapon->SetPercent(NewHpPercent);
	PreviousWeaponPercent = NewHpPercent;
}
