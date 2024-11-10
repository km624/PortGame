// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGHudWidget.h"
#include "Interface/PGHudWidgetInterface.h"
#include "UI/PGHPBarWidget.h"
#include "Components/Image.h"

UPGHudWidget::UPGHudWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPGHudWidget::SetUpWaidget(const FPGCharacterStat& BaseStat, const FPGCharacterStat& ModifierStat, float MaxHitGauge)
{
	BP_HpBar->SetUpWaidget(BaseStat, ModifierStat,MaxHitGauge);
}

void UPGHudWidget::UpdateHpBar(float NewCurrentHp)
{
	BP_HpBar->UpdateHpBar(NewCurrentHp);
}

void UPGHudWidget::UpdateHitGaugeBar(float NewHitGauge)
{
	BP_HpBar->UpdateHitGaugeBar(NewHitGauge);
}

void UPGHudWidget::CorssHairEnable(bool bIsaim)
{
	if (bIsaim)
		Image_CrossHair->SetVisibility(ESlateVisibility::Visible);
	else
		Image_CrossHair->SetVisibility(ESlateVisibility::Hidden);

	
}

void UPGHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	////위젯에 있는 hp바
	//HpBar = Cast<UABHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	//ensure(HpBar);
	IPGHudWidgetInterface* HUDPawn = Cast<IPGHudWidgetInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetUpHudWidget(this);
	}


}
