// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PGHudWidget.h"
#include "Interface/PGHudWidgetInterface.h"
#include "UI/PGHPBarWidget.h"
#include "Components/Image.h"
#include "UI/PGUltiSkillGaugeWidget.h"
#include "UI/SkillWidget.h"
#include "UI/PGDashWidget.h"
#include "UI/PGGunWidget.h"
#include "UI/PGReloadWidget.h"
#include "PortGame/PortGame.h"
#include "UI/PGFieldUserWidget.h"
#include "UI/PGAllCharactersWidget.h"
#include "UI/PGMiniMapWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "AI/PGAIController.h"




UPGHudWidget::UPGHudWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	OriginTint = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	RedTint = FLinearColor(1.0f, 0.0f, 0.1f, 0.5f);
}

void UPGHudWidget::SetOwingCharcter(AActor* owningCharacter)
{
	
	OwningCharacter = owningCharacter;
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

void UPGHudWidget::SetupDashWidget(float dashtisme)
{
	BP_DashWidget->SetUpWidget(dashtisme);
}

void UPGHudWidget::SetupGunWidget(int32 maxammo)
{
	BP_GunWidget->SetUpGunWidget(maxammo);
}

void UPGHudWidget::SetupReloadWidget(float reloadtime)
{
	BP_ReloadWidget->SetUpReloadWidget(reloadtime);
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

void UPGHudWidget::StartDash()
{
	BP_DashWidget->OnDash();
}

void UPGHudWidget::UpdateGunAmmo(int32 Newammo)
{
	BP_GunWidget->UpdateGunAmmo(Newammo);
}

void UPGHudWidget::StartReload(bool reload, float reloadtime)
{
	BP_ReloadWidget->StartReloadtime(reload, reloadtime);
	bIsReload = reload;
	if (reload)
	{
		Image_CrossHair->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		if(bIsAim)
			Image_CrossHair->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void UPGHudWidget::SetGunWidgetEnable(bool bIsaim)
{
	bIsAim = bIsaim;
	if (bIsaim)
	{
		if(bIsReload==false)
			Image_CrossHair->SetVisibility(ESlateVisibility::Visible);
		BP_GunWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image_CrossHair->SetVisibility(ESlateVisibility::Hidden);
		BP_GunWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void UPGHudWidget::ChangeCrosshair(bool bInRange)
{
	if (bInRange)
	{
		
		Image_CrossHair->SetColorAndOpacity(OriginTint);
	}
	else
	{
		
		Image_CrossHair->SetColorAndOpacity(RedTint);
	}
}

void UPGHudWidget::SetUpAllCharactersWidget(int32 num)
{
	BP_AllCharacterWidget->SetUpCharacters(num);
}

void UPGHudWidget::OnStartChangeCooltime(int32 num)
{
	BP_AllCharacterWidget->OnChangeCharacter(num);
}


void UPGHudWidget::SetupFieldGauge(uint8 teamid, float maxgauge, float currentgauge)
{
	BP_FieldGauge->SetUpWidget(teamid, maxgauge, currentgauge);
}

void UPGHudWidget::UpdateFieldGague(float newGauge)
{
	BP_FieldGauge->UpdateFieldGauge(newGauge);
}

void UPGHudWidget::CollapsedFieldGauge()
{
	BP_FieldGauge->SetLockImage(false);
	BP_FieldGauge->SetVisibility(ESlateVisibility::Collapsed);
}

void UPGHudWidget::SetUpLockImage(bool lock)
{
	BP_FieldGauge->SetLockImage(lock);
}

void UPGHudWidget::SetupCharacterMinimap(int8 mynum, const TArray<AActor*>& ActorArray, const TArray<APGAIController*>& allaicontrollers)
{
	BP_MiniMap->SetupPlayers(mynum, ActorArray, allaicontrollers);
}

void UPGHudWidget::ChangeMiniMapSize(bool bisminimap)
{

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(BP_MiniMap->Slot);
	if (CanvasSlot)
	{
		if (bisminimap)
		{
			CanvasSlot->SetSize(FVector2D(DefaultMiniMapXSize, DefaultMiniMapYSize));

			CanvasSlot->SetAnchors(FAnchors(1.0f, 0.0f, 1.0f, 0.0f));
			CanvasSlot->SetAlignment(FVector2D(1.0f, 0.0f));

			CanvasSlot->SetPosition(FVector2D(-50.0f, 50.0f));
			
		}
		else
		{
			CanvasSlot->SetSize(FVector2D(DefaultMiniMapXSize*2.0f, DefaultMiniMapYSize*2.0f));
			CanvasSlot->SetAnchors(FAnchors(0.5f,0.5f));
			CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
			CanvasSlot->SetPosition(FVector2D(0.0f, 0.0f));		
		}

		BP_MiniMap->ChangeMinimapSize(bisminimap);
	}
}





void UPGHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	////SLOG(TEXT("HUD : NativeConstruct"));
	//IPGHudWidgetInterface* HUDPawn = Cast<IPGHudWidgetInterface>(OwningCharacter);
	//SLOG(TEXT("%s : SetUpHudWidget"), *OwningCharacter->GetActorNameOrLabel());
	//if (HUDPawn)
	//{
	//	
	//	HUDPawn->SetUpHudWidget(this);
	//}


}
