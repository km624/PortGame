// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGCharcterWidget.h"
#include "Data/CharacterEnumData.h"
#include "UI/PGHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Engine/Texture2D.h"

//#include "Styling/SlateBrush.h"
//#include "Styling/WidgetStyle.h"
#include "Slate/SlateBrushAsset.h"
#include "PortGame/PortGame.h"


void UPGCharcterWidget::SetStyle(UTexture2D* charcterimage)
{

	FProgressBarStyle ProgressBarStyle;
	FLinearColor backgroundcolor = FLinearColor(FVector(0.01f, 0.01f, 0.01f));
	FLinearColor Fillcolor = FLinearColor(FVector(1.0f, 1.0f, 1.0f));
	
	CharacerImage = charcterimage;

	BackgroundBrush.SetResourceObject(CharacerImage);
	FillBrush.SetResourceObject(CharacerImage);
	
	// 배경 브러시 설정
	ProgressBarStyle.BackgroundImage = BackgroundBrush;
	ProgressBarStyle.BackgroundImage.TintColor = FSlateColor(backgroundcolor);
	// 채워지는 브러시 설정
	ProgressBarStyle.FillImage = FillBrush;
	ProgressBarStyle.FillImage.TintColor = FSlateColor(Fillcolor);

	// 스타일 적용
	ProgressBar_Character->SetWidgetStyle(ProgressBarStyle);
}

void UPGCharcterWidget::CheckIsMine(bool bismine)
{
	if (bismine)
	{
		BP_HpBar->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		BP_HpBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPGCharcterWidget::SetChangeCoolTime(float cooltime)
{
	Changecooltime = cooltime;

	Currentcooltime = cooltime;
	SLOG(TEXT("cool : %f"), cooltime);

	if (ProgressBar_Character)
	{
		ProgressBar_Character->SetPercent(Changecooltime / Currentcooltime);
	}
}

void UPGCharcterWidget::SetUphpbarWidget(const FPGCharacterStat& Stat, const FPGCharacterStat& ModifierStat)
{
	BP_HpBar->SetUpWidget(Stat, ModifierStat);
}

void UPGCharcterWidget::UpdateHpBar(float NewCurrentHp)
{
	BP_HpBar->UpdateHpBar(NewCurrentHp);
}

void UPGCharcterWidget::UpdateHitGaugeBar(float NewHitGauge)
{
	BP_HpBar->UpdateHitGaugeBar(NewHitGauge);
}

void UPGCharcterWidget::SetDead(int8 team)
{
	FLinearColor Fillcolor = FLinearColor(FVector(0.2f, 0.2f, 0.2f));
	ProgressBar_Character->SetFillColorAndOpacity(Fillcolor);
	BP_HpBar->SetVisibility(ESlateVisibility::Collapsed);
}

void UPGCharcterWidget::StartChangeCooltime()
{
	
	GetWorld()->GetTimerManager().ClearTimer(ChangeCooltimeTimerHandle);

	FLinearColor Fillcolor = FLinearColor(FVector(0.25f, 0.25f, 0.25f));
	ProgressBar_Character->SetFillColorAndOpacity(Fillcolor);

	Currentcooltime = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(ChangeCooltimeTimerHandle, this, &UPGCharcterWidget::UpdateChangeCooltime, 0.01f, true);
	
	

}

void UPGCharcterWidget::UpdateChangeCooltime()
{
	const float Duration = Changecooltime;
	const float DeltaTime = 0.01f;
	const float Increment = DeltaTime / Duration;
	
	Currentcooltime += Increment;
	if (ProgressBar_Character)
	{
		ProgressBar_Character->SetPercent(FMath::Clamp(Currentcooltime, 0.0f, 1.0f));
	}

	if (Currentcooltime >= 1.0f)
	{
		FLinearColor Fillcolor = FLinearColor(FVector(1.0f, 1.0f, 1.0f));
		ProgressBar_Character->SetFillColorAndOpacity(Fillcolor);
		GetWorld()->GetTimerManager().ClearTimer(ChangeCooltimeTimerHandle);
		
		
		
		
	}
}
