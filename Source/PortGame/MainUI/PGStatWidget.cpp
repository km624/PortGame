// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI/PGStatWidget.h"
#include "MainUI/PGStatProgressBarWidget.h"
#include "MainUI/PGGunStatProgressBarWidget.h"
#include "Components/VerticalBox.h"
#include "Data/PlayerCharacterDataAsset.h"
#include "Engine/GameInstance.h"
#include "GameInstance/PGGameInstanceInterface.h"
#include "Data/GunWeaponData.h"
#include "Components/VerticalBox.h"


void UPGStatWidget::SetUpAllProgressBar()
{
	

	BP_StatProgressBar_HP->SetupCharacterStat(MaxHp,TEXT("HP"));
	BP_StatProgressBar_Attack->SetupCharacterStat(MaxAttack, TEXT("Damage"));
	BP_StatProgressBar_Range->SetupCharacterStat(MaxAttackRange,  TEXT("Range"));
	BP_StatProgressBar_AttackSpeed->SetupCharacterStat(MaxAttackSpeed, TEXT("AttackSpeed"));
	BP_StatProgressBar_HitGauge->SetupCharacterStat(MaxHitGauge, TEXT("HitGauge"));

	BP_GunProgressBar_MaxAmmo->SetupGunStat(MaxAmmo, TEXT("Ammo"));
	BP_GunProgressBar_Reload->SetupGunStat(MaxReload, TEXT("ReloadTime"));
	BP_GunProgressBar_ShootInterval->SetupGunStat(MaxShootInterval, TEXT("ShootInterval"));
	BP_GunProgressBar_TraceDistanced->SetupGunStat(MaxTraceDistance, TEXT("TraceDistance"));

		
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

void UPGStatWidget::UpdateWeaponStat(UWeaponData* weapondata)
{
	FPGCharacterStat weaponStat;
	weaponStat = weapondata->ModifierStat;

	BP_StatProgressBar_HP->UpdateWeapon(weaponStat.MaxHp);
	BP_StatProgressBar_Attack->UpdateWeapon(weaponStat.Attack);
	BP_StatProgressBar_Range->UpdateWeapon(weaponStat.AttackRange);
	BP_StatProgressBar_AttackSpeed->UpdateWeapon(weaponStat.AttackSpeed);
	BP_StatProgressBar_HitGauge->UpdateWeapon(weaponStat.HitGauge);
	
	UGunWeaponData* gundata = Cast<UGunWeaponData>(weapondata);
	if (gundata)
	{
		FPGGunStat gunstat = gundata->GunStat;
		
		VerticalBox_Gun->SetVisibility(ESlateVisibility::Visible);


		BP_GunProgressBar_MaxAmmo->UpdateGunStat(gunstat.ammoMaxCount);
		BP_GunProgressBar_Reload->UpdateGunStat(gunstat.reloadingTime);
		BP_GunProgressBar_ShootInterval->UpdateGunStat(gunstat.ShootInterval);
		BP_GunProgressBar_TraceDistanced->UpdateGunStat(gunstat.traceDistance);
	}
	
}


