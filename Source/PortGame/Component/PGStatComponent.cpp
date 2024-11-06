// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PGStatComponent.h"


UPGStatComponent::UPGStatComponent()
{
	//const UDataTable* DataTable;
	static ConstructorHelpers::FObjectFinder<UDataTable>DT_STAT(TEXT("/Script/Engine.DataTable'/Game/PortGame/Data/Stat/DT_CharacterStat.DT_CharacterStat'"));
	if (DT_STAT.Object)
	{
		UDataTable* characterstatDataTable = DT_STAT.Object;

		//// 행을 하나씩 얻는 방법
		//const FName normal = TEXT("Normal");
		//const FName rare = TEXT("Rare");

		FString contextString;
		//const FPGCharacterStat* normalInfo = characterstatDataTable->FindRow<FPGCharacterStat>(normal, contextString);
		//const FPGCharacterStat* normalInfo = characterstatDataTable->FindRow<FPGCharacterStat>(normal, contextString);

		//// 행에 대한 모든 내용을 얻는 방법
		//TArray<FPGCharacterStat*> infos;
		//characterstatDataTable->GetAllRows< FPGCharacterStat>(contextString, infos);

		//행의 네임을 가져와서 그 순 차례대로 하나씩 찾는법으로 map에 추가
		TArray<FName> rowNames = characterstatDataTable->GetRowNames();

		for (FName rowName : rowNames)
		{
			const FPGCharacterStat* rowInfo = characterstatDataTable->FindRow<FPGCharacterStat>(rowName, contextString);
			AllStat.Add(rowName, *rowInfo);
		}	
	}


	
	
	bWantsInitializeComponent = true;
	
}

void UPGStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetBaseStat(TEXT("Normal"));
	
	SetHp(BaseStat.MaxHp);

}

void UPGStatComponent::SetHp(float NewHp)
{
	CurrentHp = NewHp;

	OnHpChanged.Broadcast(NewHp);

	UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentHp);
	if (CurrentHp <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("죽음"));
		OnHpZero.Broadcast();
	}
}

//void UPGStatComponent::SetStat(FName rarity)
//{
//	 BaseStat =  AllStat[rarity];
//}

void UPGStatComponent::SetLevelCharacter(int32 level)
{
}

void UPGStatComponent::Damaged(float Damage)
{
	
	SetHp(CurrentHp-Damage);
	
}




