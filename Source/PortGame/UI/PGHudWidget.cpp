// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGHudWidget.h"
#include "Interface/PGHudWidgetInterface.h"
#include "UI/PGHPBarWidget.h"
#include "Components/Image.h"
#include "UI/PGUltiSkillGaugeWidget.h"
#include "UI/SkillWidget.h"

UPGHudWidget::UPGHudWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UPGHudWidget::SetUpWidget(const FPGCharacterStat& BaseStat, const FPGCharacterStat& ModifierStat)
{
	BP_HpBar->SetUpWidget(BaseStat, ModifierStat);
}

void UPGHudWidget::SetupUltiSkillWidget(const float currentGague)
{
	BP_UltiSkillGauge->SetUpWidget(currentGague);
}

void UPGHudWidget::SetupSkillWidget(float cooltime)
{
	BP_SkillWidget->SetUpWidget(cooltime);
}

void UPGHudWidget::UpdateHpBar(float NewCurrentHp)
{
	BP_HpBar->UpdateHpBar(NewCurrentHp);
}

void UPGHudWidget::UpdateHitGaugeBar(float NewHitGauge)
{
	BP_HpBar->UpdateHitGaugeBar(NewHitGauge);
}

void UPGHudWidget::UpdateUltiSkillGaugeBar(float newUltiGauge)
{
	BP_UltiSkillGauge->UpdateUltiSkillGauge(newUltiGauge);
}

void UPGHudWidget::StartSkillCoolTime(bool start)
{
	BP_SkillWidget->StartSkillCooltime(start);
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

	
	IPGHudWidgetInterface* HUDPawn = Cast<IPGHudWidgetInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetUpHudWidget(this);
	}


}
