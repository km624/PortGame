// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PGStatComponent.h"


UPGStatComponent::UPGStatComponent()
{
	//const UDataTable* DataTable;
	static ConstructorHelpers::FObjectFinder<UDataTable>DT_STAT(TEXT("/Script/Engine.DataTable'/Game/PortGame/Data/Stat/DT_CharacterStat.DT_CharacterStat'"));
	if (DT_STAT.Object)
	{
		UDataTable* characterstatDataTable = DT_STAT.Object;

		FString contextString;
	
		//행의 네임을 가져와서 그 순 차례대로 하나씩 찾는법으로 map에 추가
		TArray<FName> rowNames = characterstatDataTable->GetRowNames();

		for (FName rowName : rowNames)
		{
			const FPGCharacterStat* rowInfo = characterstatDataTable->FindRow<FPGCharacterStat>(rowName, contextString);
			AllStat.Add(rowName, *rowInfo);
		}	

	}

	CurrentCharacterRarity = TEXT("Normal");
	
	
	bWantsInitializeComponent = true;
	
}

void UPGStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetMaxHitGauge(15.0f);
	SetCurrentRarity(CurrentCharacterRarity);

	SetHp(GetTotalStat().MaxHp);

	SetHitGauge(MaxHitGauge);

}

void UPGStatComponent::SetCurrentRarity(FName rarity)
{
	if(AllStat.Find(rarity))
		SetBaseStat(CurrentCharacterRarity);
}

void UPGStatComponent::SetHp(float NewHp)
{
	if (NewHp <= 0)
	{
		CurrentHp = 0;
	}
	else
		CurrentHp = NewHp;

	OnHpChanged.Broadcast(CurrentHp);

	

}

void UPGStatComponent::SetHitGauge(float NewHitGauge)
{
	if (NewHitGauge <= 0)
	{
		CurretHitGauge = 0;
	}
	else
		CurretHitGauge = NewHitGauge;

	
	OnHitGaugeChanged.Broadcast(CurretHitGauge);
	//UE_LOG(LogTemp, Warning, TEXT("current hitGauge:  %f"), CurretHitGauge);

}



void UPGStatComponent::SetLevelCharacter(int32 level)
{

}

void UPGStatComponent::ResetHitGauge()
{
	SetHitGauge(MaxHitGauge);
}

void UPGStatComponent::Damaged(float Damage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(Damage, 0, Damage);

	const float PrevHitGauge = CurretHitGauge;
	

	SetHp(PrevHp - ActualDamage);
	SetHitGauge(PrevHitGauge - ActualDamage);

	if (CurretHitGauge <= KINDA_SMALL_NUMBER)
	{
		//UE_LOG(LogTemp, Warning, TEXT("GaugeEnd"));
		OnHitGaugeZero.Broadcast();
		ResetHitGauge();
	}
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stat : Dead"));
		OnHpZero.Broadcast();
	}
	
}




