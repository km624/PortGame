// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/mainPlayerController.h"
#include "Engine/AssetManager.h"
#include "Data/PlayerCharacterDataAsset.h"
#include "PortGame/PortGame.h"
#include "Data/BaseCharacterDataAsset.h"

AmainPlayerController::AmainPlayerController()
{
}

void AmainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AllFindCharacterData();

}

void AmainPlayerController::AllFindCharacterData()
{
	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;

	//에셋 아이디 리스트에서 태그 아이디를 가지고 있는 애를 배열로 반환
	Manager.GetPrimaryAssetIdList(TEXT("PlayerData"), Assets);

	SLOG(TEXT("%d"), Assets.Num());
	ensure(0 < Assets.Num());

	for(FPrimaryAssetId& playerAsset : Assets)
	{
		FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(playerAsset));
		if (AssetPtr.IsPending())
		{
			AssetPtr.LoadSynchronous();
		}

		//UPlayerCharacterDataAsset* palyerData= Cast<UPlayerCharacterDataAsset>(AssetPtr.Get());
		UBaseCharacterDataAsset* BaserData= Cast<UBaseCharacterDataAsset>(AssetPtr.Get());

		if (IsValid(BaserData))
		{
			AllBaseDatas.Add(BaserData);
		}
	}


}
