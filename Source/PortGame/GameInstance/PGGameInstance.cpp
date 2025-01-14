// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/PGGameInstance.h"
#include "Data/PlayerCharacterDataAsset.h"

UPGGameInstance::UPGGameInstance()
{
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
}

FPGCharacterStat UPGGameInstance::GetStat(FName rarity)
{
	return AllStat.Contains(rarity) ? AllStat[rarity] : FPGCharacterStat();
}


