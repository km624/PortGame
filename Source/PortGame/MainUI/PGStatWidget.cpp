// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI/PGStatWidget.h"
#include "MainUI/PGStatProgressBarWidget.h"
#include "MainUI/PGGunStatProgressBarWidget.h"
#include "Components/VerticalBox.h"
#include "Data/PlayerCharacterDataAsset.h"
#include "Engine/GameInstance.h"
#include "GameInstance/PGGameInstanceInterface.h"


void UPGStatWidget::SetUpAllProgressBar()
{
	

	BP_StatProgressBar_HP->SetupCharacterStat(MaxHp,TEXT("HP"));
	BP_StatProgressBar_Attack->SetupCharacterStat(MaxAttack, TEXT("Damage"));
	BP_StatProgressBar_Range->SetupCharacterStat(MaxAttackRange,  TEXT("Range"));
	BP_StatProgressBar_AttackSpeed->SetupCharacterStat(MaxAttackSpeed, TEXT("AttackSpeed"));
	BP_StatProgressBar_HitGauge->SetupCharacterStat(MaxHitGauge, TEXT("HitGauge"));

	//BP_GunProgressBar_MaxAmmo
		
}

void UPGStatWidget::UpdateBaseStat(UPlayerCharacterDataAsset* characterData)
{

	FPGCharacterStat characterStat;
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance)
	{
		IPGGameInstanceInterface* pggameinstance = Cast<IPGGameInstanceInterface>(GameInstance);

		if (pggameinstance)
		{

			characterStat = pggameinstance->GetStat(characterData->Rarity);

		}
	}

	BP_StatProgressBar_HP->UpdateBase(characterStat.MaxHp);
	BP_StatProgressBar_Attack->UpdateBase(characterStat.Attack);
	BP_StatProgressBar_Range->UpdateBase(characterStat.AttackRange);
	BP_StatProgressBar_AttackSpeed->UpdateBase(characterStat.AttackSpeed);
	BP_StatProgressBar_HitGauge->UpdateBase(characterStat.HitGauge);
}


