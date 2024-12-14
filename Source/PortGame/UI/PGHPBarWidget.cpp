// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/PGSetHpbarCharacterInterface.h"
#include "PortGame/PortGame.h"

UPGHPBarWidget::UPGHPBarWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	MaxHp = -1.0f;
	MaxHitGauge = -1.0f;
	PreviousHpPercent = -1.0f;
	PreviousHitGaugePercent = -1.0f;

}


void UPGHPBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	this->SetVisibility(ESlateVisibility::Hidden);

	IPGSetHpbarCharacterInterface* CharacterWidget = Cast<IPGSetHpbarCharacterInterface>(OwningActor);
	if (CharacterWidget)
	{
		//액터의 함수 실행
		CharacterWidget->SetUpHpWidget(this);
	}
}

void UPGHPBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SmoothingHp(InDeltaTime);
	SmoothingHitGauge(InDeltaTime);
}	

void UPGHPBarWidget::SetUpWidget(const FPGCharacterStat& Stat, const FPGCharacterStat& ModifierStat)
{
	
	MaxHp = (Stat + ModifierStat).MaxHp;
	CurrentHp = MaxHp;
	MaxHitGauge = (Stat + ModifierStat).HitGauge;
	CurrentHitGauge = MaxHitGauge;

	if (ProgressBar_HpBar)
	{
		ProgressBar_HpBar->SetPercent(CurrentHp / MaxHp);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("progress miss"));
	}

	if (ProgressBar_HitGauge)
	{

		ProgressBar_HitGauge->SetPercent(CurrentHitGauge / MaxHitGauge);

	}

	if(TextBlock_Hp)
	{
		TextBlock_Hp->SetText(FText::FromString(
			FString::Printf(TEXT("%.0f/%0.f"), CurrentHp, MaxHp)));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Textblocks miss"));
	}
	this->SetVisibility(ESlateVisibility::Visible);
}

void UPGHPBarWidget::UpdateHpBar(float NewCurrentHp)
{
	//이전 백분율
	PreviousHpPercent = CurrentHp / MaxHp;

	//CurrentHp = NewCurrentHp;

	ensure(MaxHp > 0.0f);
	if (ProgressBar_HpBar)
	{
		
		CurrentHp = NewCurrentHp;
		//ProgressBar_HpBar->SetPercent(CurrentHp / MaxHp);


	}
	if (TextBlock_Hp)
	{
		TextBlock_Hp->SetText(FText::FromString(
			FString::Printf(TEXT("%.0f/%0.f"), CurrentHp, MaxHp)));
	}


}

void UPGHPBarWidget::UpdateHitGaugeBar(float NewHitGauge)
{
	PreviousHitGaugePercent = CurrentHitGauge / MaxHitGauge;
	//CurrenHitGauge = NewHitGauge;
	//ensure(CurrenHitGauge > 0.0f);
	if (ProgressBar_HitGauge)
	{
		CurrentHitGauge = NewHitGauge;
		//ProgressBar_HitGauge->SetPercent(CurrenHitGauge / MaxHitGauge);

	}
}

void UPGHPBarWidget::SmoothingHp(float deltatime)
{
	
	float CurrentHpPercent = CurrentHp / MaxHp;
	
	float NewHpPercent = FMath::FInterpTo(PreviousHpPercent, CurrentHpPercent, deltatime, 2.0f);
	ProgressBar_HpBar->SetPercent(NewHpPercent);
	PreviousHpPercent = NewHpPercent;
}

void UPGHPBarWidget::SmoothingHitGauge(float deltatime)
{
	float CurrentHitGaugePercent = CurrentHitGauge / MaxHitGauge;
	float newHitGaugePercent = FMath::FInterpTo(PreviousHitGaugePercent, CurrentHitGaugePercent, deltatime, 2.0f);
	ProgressBar_HitGauge->SetPercent(newHitGaugePercent);
	PreviousHitGaugePercent = newHitGaugePercent;
}
