// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/mainPlayerController.h"
#include "Engine/AssetManager.h"
#include "Data/PlayerCharacterDataAsset.h"
#include "PortGame/PortGame.h"
#include "Data/BaseCharacterDataAsset.h"
#include "MainUI/PGMainWidget.h"
#include "Character/PGPlayerCharacter.h"
#include "Data/WeaponData.h"
#include "Data/CharacterEnumData.h"
#include "MainUI/PGSelectWidget.h"
#include "Save/PGSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"


AmainPlayerController::AmainPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPGMainWidget> mainwidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/Main/BP_MainWidget.BP_MainWidget_C'"));
	if (mainwidgetClass.Class)
	{
		MainWidgetClass = mainwidgetClass.Class;
	}
	static ConstructorHelpers::FClassFinder<UPGSelectWidget> selectwidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/Main/BP_SelectWidget.BP_SelectWidget_C'"));
	if (selectwidgetClass.Class)
	{
		SelectWidgetClass = selectwidgetClass.Class;
	}

	SelectNum = 0;
	
}

void AmainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SelectPlayerDatasMap.Add(0, nullptr);
	SelectPlayerDatasMap.Add(1, nullptr);
	SelectPlayerDatasMap.Add(2, nullptr);

	SpawnCharacters.Add(0, nullptr);
	SpawnCharacters.Add(1, nullptr);
	SpawnCharacters.Add(2, nullptr);


	

	bShowMouseCursor = true;

	AddSpawnLocation(FVector(120.0f, 0.0f, 580.0f),FRotator(0.0f, -180.0f, 0.0f));
	AddSpawnLocation(FVector(170.0f, -140.0f, 580.0f),FRotator(0.0f, 130.0f, 0.0f));
	AddSpawnLocation(FVector(170.0f, 140.0f, 580.0f),FRotator(0.0f, -150.0f, 0.0f));

}

void AmainPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	
	AllFindCharacterData();

	FindSwordData();

	FindGunData();

	LoadSaveFile();

	SetUpMainWidget();
}

void AmainPlayerController::AllFindCharacterData()
{
	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;

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

		UPlayerCharacterDataAsset* PlayerData= Cast<UPlayerCharacterDataAsset>(AssetPtr.Get());

		if (IsValid(PlayerData))
		{
			AllPlayerDatas.Add(PlayerData);
		}
	}

}

void AmainPlayerController::SetUpMainWidget()
{

	for (int32 i = 0; i < SelectPlayerDatasMap.Num(); i++)
	{
		if (SelectPlayerDatasMap[i] != nullptr)
		{
			SelectPlayerDatasMap[i] = nullptr;

			if (SpawnCharacters[i] != nullptr)
			{
				SpawnCharacters[i]->DestroyWeapontoAtackComp();
				SpawnCharacters[i]->Destroy();

				SpawnCharacters[i] = nullptr;
			}

		}
	}


	if (SelectWidget)
	{
		SelectWidget->RemoveFromParent();
		SelectWidget = nullptr;
	}
		

	if (MainWidgetClass)
	{
		MainWidget = CreateWidget<UPGMainWidget>(this, MainWidgetClass);
		if (MainWidget)
		{
			
			MainWidget->SetUpMainWidget();
			MainWidget->AddToViewport();

		}
	}
}

void AmainPlayerController::SetUpSelectWidget()
{
	if (MainWidget)
	{
		MainWidget->RemoveFromParent();
		MainWidget = nullptr;
		//MainWidget->SetVisibility(ESlateVisibility::Hidden);
	}
		

	if (SelectWidgetClass)
	{
		SelectWidget = CreateWidget<UPGSelectWidget>(this, SelectWidgetClass);
		if (SelectWidget)
		{

			SelectWidget->SetUpPlayerButton(AllPlayerDatas);

			SelectWidget->SetUpSwordData(SwordDatas);

			SelectWidget->SetUpGunData(GunDatas);

			SelectWidget->SetUpAllStatWidget();

			SelectWidget->AddToViewport();

		}
	}
}


bool AmainPlayerController::SetSelectCharcterData(UPlayerCharacterDataAsset* characterData)
{
	//선택되면 Return true;

	const int8* selectPlayerNum = SelectPlayerDatasMap.FindKey(characterData);
	
	//셀렉트 되있는 캐릭터중에 없는경우
	if (selectPlayerNum == nullptr)
	{
		//이미 3명 셀렉트가 되있는경우
		if (!SelectPlayerDatasMap.FindKey(nullptr))
		{
			SLOG(TEXT("FULL!!"));
			return false;
		}

		for (TPair<int8, UPlayerCharacterDataAsset*>& select : SelectPlayerDatasMap)
		{
			if (select.Value == nullptr)
			{

				select.Value = characterData;
				SLOG(TEXT("%s  -> %d "), *characterData->GetMeshNameAsString(), select.Key);
				//SpawnCharacter(select.Key);

				SelectNum = select.Key;
				ShowSelectWeaponWidget(characterData->Charactertype);
				break;
			}

		}

		return true;
	}
	//셀렉트 되있는 캐릭터중에 있는경우
	//다시 클릭해서 취소한다는것
	else
	{
		SelectPlayerDatasMap[*selectPlayerNum] = nullptr;
		
		SpawnCharacters[*selectPlayerNum]->DestroyWeapontoAtackComp();
		SpawnCharacters[*selectPlayerNum]->Destroy();

		SpawnCharacters[*selectPlayerNum] = nullptr;

		SLOG(TEXT("Select Back"));

		return false;
	}

	
}

void AmainPlayerController::AddSpawnLocation(FVector location, FRotator rotator)
{
	FVector Location  = location;

	
	FRotator Rotation = rotator;

	
	FTransform NewTransform(Rotation, Location);

	
	SpawnLocation.Add(NewTransform);
}

void AmainPlayerController::SpawnCharacter(int8 num)
{
	APGPlayerCharacter* character = Cast<APGPlayerCharacter>((GetWorld()->SpawnActorDeferred<APGPlayerCharacter>(
		APGPlayerCharacter::StaticClass(),
		SpawnLocation[num],
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	)));

	if (character)
	{
		character->SetupCharacterDataAsset(SelectPlayerDatasMap[num]);

		character->SetGravityNone();
		
	}
	character->FinishSpawning(SpawnLocation[num]);

	SpawnCharacters[num] = character;
}

void AmainPlayerController::ShowSelectWeaponWidget(EPlayerCharacterType characterType)
{
	SelectWidget->ShowSelectWeaponWidget(characterType);
}

void AmainPlayerController::SetSelectWeaponrData(UWeaponData* weaponData)
{
	if (weaponData)
	{
		//SelectWeaponDatasMap[SelectNum] = weaponData;

		SelectPlayerDatasMap[SelectNum]->WeaponData = weaponData;
		SelectWidget->CharacterButtonEnable();
		SelectWidget->HiddenStatWidget();
		SpawnCharacter(SelectNum);
	}
}

void AmainPlayerController::FindGunData()
{
	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;

	//에셋 아이디 리스트에서 태그 아이디를 가지고 있는 애를 배열로 반환
	Manager.GetPrimaryAssetIdList(TEXT("GunData"), Assets);

	SLOG(TEXT("Gun %d"), Assets.Num());
	ensure(0 < Assets.Num());

	for (FPrimaryAssetId& playerAsset : Assets)
	{
		FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(playerAsset));
		if (AssetPtr.IsPending())
		{
			AssetPtr.LoadSynchronous();
		}

		UWeaponData* gunData = Cast<UWeaponData>(AssetPtr.Get());

		if (IsValid(gunData))
		{
			GunDatas.Add(gunData);
		}
	}

}

void AmainPlayerController::FindSwordData()
{
	UAssetManager& Manager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Assets;

	//에셋 아이디 리스트에서 태그 아이디를 가지고 있는 애를 배열로 반환
	Manager.GetPrimaryAssetIdList(TEXT("SwordData"), Assets);

	SLOG(TEXT("Sword %d"), Assets.Num());
	ensure(0 < Assets.Num());

	for (FPrimaryAssetId& playerAsset : Assets)
	{
		FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(playerAsset));
		if (AssetPtr.IsPending())
		{
			AssetPtr.LoadSynchronous();
		}

		UWeaponData* swordData = Cast<UWeaponData>(AssetPtr.Get());

		if (IsValid(swordData))
		{
			SwordDatas.Add(swordData);
		}
	}

}

void AmainPlayerController::SelectComplete()
{
	TArray<UPlayerCharacterDataAsset*> selectCharacter;

	for (TPair<int8, UPlayerCharacterDataAsset*>& select : SelectPlayerDatasMap)
	{
		if (select.Value != nullptr)
		{

			selectCharacter.Add(select.Value);
			
		}

	}

	if (selectCharacter.Num() == 0)
	{
		SLOG(TEXT("Not Select!!"));
		return;
	}

	
	for (UPlayerCharacterDataAsset* select : selectCharacter)
	{
		SLOG(TEXT("sleelct : %s"), *select->GetMeshNameAsString());
		//gameinstance->SetCharacterData(select);
		SaveGameInstance->SelectPlayerDatas.Add(select);
	}
	
	SavesaveFile();
	OnLoadGameLevel();

	
}

void AmainPlayerController::ShowCharacterStat(UPlayerCharacterDataAsset* characterData)
{
	//선택되면 Return true;

	const int8* selectPlayerNum = SelectPlayerDatasMap.FindKey(characterData);

	//셀렉트 되있는 캐릭터중에 없는경우
	if (selectPlayerNum == nullptr)
	{
		//이미 3명 셀렉트가 되있는경우
		if (!SelectPlayerDatasMap.FindKey(nullptr))
		{
			SLOG(TEXT("FULL!!"));
			return;
		}
		int8 selectNum = 0;
		for (TPair<int8, UPlayerCharacterDataAsset*>& select : SelectPlayerDatasMap)
		{
			if (select.Value == nullptr)
			{

				selectNum = select.Key;
				if (SelectWidget)
				{
					int32 level = 0;
					if (SaveGameInstance)
						level = SaveGameInstance->CharacterLevel[characterData->GetMeshNameAsString()];
					SelectWidget->UpdateStatWidget(selectNum, characterData, level);
				}

				break;
			}

		}
	}
	//셀렉트 되있는 캐릭터중에 있는경우
	//다시 클릭해서 취소한다는것
	else
	{

		SLOG(TEXT("Already Select"));

	}

}


void AmainPlayerController::ShowWeaponStat(UWeaponData* weapondata)
{
	//선택되면 Return true;

	if (weapondata)
	{
		SelectWidget->UpdateWeaponStatWidget(SelectNum, weapondata);


	}
}

void AmainPlayerController::LoadSaveFile()
{

	SaveGameInstance = Cast<UPGSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0));
	if (SaveGameInstance)
	{
		if (AllPlayerDatas.Num() > 0)
		{
			for (UPlayerCharacterDataAsset* playerdata : AllPlayerDatas)
			{
				if (SaveGameInstance->CharacterLevel.Contains(playerdata->GetMeshNameAsString()))
				{
					FString charactername = playerdata->GetMeshNameAsString();
					SLOG(TEXT("%s  : %d "), *charactername, SaveGameInstance->CharacterLevel[charactername]);
				}
				else
				{
					FString charactername = playerdata->GetMeshNameAsString();
					SaveGameInstance->CharacterLevel.Add(playerdata->GetMeshNameAsString(), 5);
					SLOG(TEXT("%s  : %d "), *charactername, SaveGameInstance->CharacterLevel[charactername]);
				}

			}
			SLOG(TEXT("LoadComplete"));
		}

		//SelectPlayer 초기화
		SaveGameInstance->SelectPlayerDatas.Empty();
	}
	else
	{
		SaveGameInstance = NewObject<UPGSaveGame>();
		SLOG(TEXT("CreateSaveFile"));
		if (AllPlayerDatas.Num() > 0)
		{
			for (UPlayerCharacterDataAsset* playerdata : AllPlayerDatas)
			{
				SaveGameInstance->CharacterLevel.Add(playerdata->GetMeshNameAsString(), 5);
			}
		}

		SavesaveFile();
	}
}

void AmainPlayerController::SavesaveFile()
{
	if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0))
	{
		SLOG(TEXT("Save Error!!"));
	}
}

void AmainPlayerController::OnLoadGameLevel()
{
	//FLatentActionInfo LatentInfo;
	//LatentInfo.CallbackTarget = this;
	//LatentInfo.ExecutionFunction = FName("OnLevelLoadCompleted"); // 로딩 완료 후 호출될 함수
	//LatentInfo.Linkage = 0;
	//LatentInfo.UUID = 1;

	//UGameplayStatics::LoadStreamLevel(this, TEXT("Asian_Village_Copy1"), true, true, LatentInfo);

	
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	FString AssetPath = FString::Printf(TEXT("/Game/PortGame/Level/Asian_Village_Copy.Asian_Village_Copy"));
	//FSoftObjectPath LevelRef(TEXT("/Game/PortGame/Level/Asian_Village_Copy1.Asian_Village_Copy1"));
	FSoftObjectPath LevelRef(AssetPath);

	StreamableManager.RequestAsyncLoad(LevelRef, FStreamableDelegate::CreateUObject(this, &ThisClass::OnLevelLoadCompleted));
	
}

void AmainPlayerController::OnLevelLoadCompleted()
{
	
	SLOG(TEXT("LoadComplete"));

	UGameplayStatics::OpenLevel(this, TEXT("Asian_Village_Copy"));
	
}

void AmainPlayerController::QuitGame()
{
	ConsoleCommand("quit");
}



