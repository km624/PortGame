// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PGPlayerController.h"

#include "UI/PGHudWidget.h"

#include "Character/PGPlayerCharacter.h"

#include "Component/PGWidgetComponent.h"

#include "PortGame/PortGame.h"

#include "AI/PGAIController.h"

#include "Save/PGSaveGame.h"

#include "Kismet/GameplayStatics.h"

#include "GameFramework/GameUserSettings.h"

APGPlayerController::APGPlayerController()
{
	
}

void APGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	

}

void APGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

}

void APGPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		int32 ScreenWidth = UserSettings->GetScreenResolution().X;
		int32 ScreenHeight = UserSettings->GetScreenResolution().Y;

		FString ResolutionText = FString::Printf(TEXT("User Resolution: %d x %d"),
			ScreenWidth,
			ScreenHeight);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, ResolutionText);
	}
	
	APGPlayerCharacter* currentplayer = Cast<APGPlayerCharacter>(aPawn);
	//PlayerCharacters.Add(currentplayer);
	SetViewTarget(GetPawn());

	currentplayer->SetCharacterInputData(EControlData::Base);
	//sSLOG(TEXT("Possess :%s"), *aPawn->GetActorNameOrLabel());
	if (currentplayer)
	{
		currentplayer->HiddenWidget();
		currentplayer->HudWidgetAddviewport();
	}
}

void APGPlayerController::PlayCameraShake(TSubclassOf<class UCameraShakeBase> camerashake)
{
	ClientStartCameraShake(camerashake);
}

void APGPlayerController::ChangedCharacterPossess(int8 playernum)
{
	APGPlayerCharacter* changeplayer = nullptr;

	if(PlayerCharacters.IsValidIndex(playernum))
		changeplayer = PlayerCharacters[playernum];
	else
	{
		SLOG(TEXT("NoCharacter!!"));
		return;
	}

	if (changeplayer == GetPawn())
	{
		SLOG(TEXT("SameCharacter"));
		return;
	}


	if (changeplayer->GetbIsDead())
	{
		SLOG(TEXT("CharacterDead"));
		return;
	}

	if (biscooltime)
	{
		SLOG(TEXT("ChangeCoolTime!!"));
		return;
	}

	APGPlayerCharacter* currentplayer = Cast<APGPlayerCharacter>(GetPawn());

	if (!currentplayer->GetbIsMiniMap())
	{
		SLOG(TEXT("Map!!!!"));
		return;
	}
	
	UnPossess();

	ChangeCharacterController(changeplayer, currentplayer);

	biscooltime = true;
	//쿨타임 세팅
	changeplayer->OnStartChangeCharacterWidget(PlayerCharacters.IndexOfByKey(changeplayer));
	
	GetWorld()->GetTimerManager().SetTimer(
		ChangedCharacterTimerHanlde,
		[this]() {
			biscooltime = false;
			GetWorld()->GetTimerManager().ClearTimer(ChangedCharacterTimerHanlde);
		}, ChangeCooltime, false
	);
	

	Possess(changeplayer);
}

void APGPlayerController::SpawnCharacterAdd(APGPlayerCharacter* character,APGAIController* aicontroller)
{
	if (character&& aicontroller)
	{

		PlayerCharacters.Add(character);
		AIPlayerControllers.Add(aicontroller);
		//SLOG(TEXT("%s : add"), *character->GetActorNameOrLabel());
	}
	
}

void APGPlayerController::SetupAllCharcterWidget()
{
	if (PlayerCharacters.Num() > 0)
	{
		for (APGPlayerCharacter* playerCharacter : PlayerCharacters)
		{
			playerCharacter->SetupAllCharacterWidget(PlayerCharacters.Num());
		}


		for (APGPlayerCharacter* playerCharacter : PlayerCharacters)
		{
			playerCharacter->SetupMyCharacterWidgetToAnother(PlayerCharacters, ChangeCooltime);

			playerCharacter->SetupPlayerMiniMap(PlayerCharacters, AIPlayerControllers);
		}
	}
}

void APGPlayerController::ChangeCharacterController(APGPlayerCharacter* newcharacter, APGPlayerCharacter* oldcharacter)
{

	if (oldcharacter)
	{
		if (PlayerCharacters.Contains(oldcharacter))
		{
			//전에 hud 지우기
			oldcharacter->RemoveHudWidget();

			//AI 컨트롤러 possess
			APGAIController* oldaicontroller = AIPlayerControllers[PlayerCharacters.IndexOfByKey(oldcharacter)];
			if (oldaicontroller)
			{
				oldaicontroller->SetAttackAIData();
				oldaicontroller->Possess(oldcharacter);
			}
		}

	}

	if (newcharacter)
	{
		if (PlayerCharacters.Contains(newcharacter))
		{

			newcharacter->SetbIsAim(false);
			newcharacter->SetbIsShoot(false);
			//ai 컨트롤러 초기화 진행
			APGAIController* newaicontroller = AIPlayerControllers[PlayerCharacters.IndexOfByKey(newcharacter)];

			if (newaicontroller)
			{
				newaicontroller->AIIsDead();
			}
		}
	}
}

void APGPlayerController::ChangeMiniMap(bool bIsMiniMap)
{
	if(bIsMiniMap)
	{
		FInputModeGameOnly GameOnlyInputMode;
		SetIgnoreLookInput(false);
		bShowMouseCursor = false;
		SetInputMode(GameOnlyInputMode);
	}
	else
	{
		FInputModeGameAndUI GameAndUI;
		SetIgnoreLookInput(true);
		bShowMouseCursor = true;
		SetInputMode(GameAndUI);
	}
}

UPGSaveGame* APGPlayerController::LoadSaveFile()
{
	SaveGameInstance = Cast<UPGSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0));
	if (SaveGameInstance)
	{
		
		return SaveGameInstance;
	}
	return nullptr;
}

void APGPlayerController::SavesaveGameFile()
{
	for (APGPlayerCharacter* playercharacter : PlayerCharacters)
	{
		int32 level = playercharacter->GetPlayerCharacterLevel();
		FString CharacterName = playercharacter->CharacterName;
		if (SaveGameInstance->CharacterLevel.Contains(CharacterName))
		{
			SaveGameInstance->CharacterLevel[CharacterName] = level;
			SLOG(TEXT("Save %s "), *CharacterName);
		}
			
	}

	if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0))
	{
		SLOG(TEXT("Save Error!!"));
	}
}

void APGPlayerController::GameClear()
{
	

	bShowMouseCursor = true;

	FInputModeGameAndUI gameandui;
	SetIgnoreLookInput(true);
	SetInputMode(gameandui);
	for (APGPlayerCharacter* playerCharacter : PlayerCharacters)
	{
		if (!playerCharacter->GetbIsDead())
			playerCharacter->UpdateGameState(true);
	}

		
		SavesaveGameFile();
	
}
void APGPlayerController::GameOver()
{
	bShowMouseCursor = true;

	FInputModeGameAndUI gameandui;
	SetIgnoreLookInput(true);
	SetInputMode(gameandui);
	for (APGPlayerCharacter* playerCharacter : PlayerCharacters)
	{
		if(!playerCharacter->GetbIsDead())
			playerCharacter->UpdateGameState(false);
	}
		SavesaveGameFile();
	
}

void APGPlayerController::ShowBodyGuardOption(bool bShowOption)
{
	if (!bShowOption)
	{
		FInputModeGameOnly GameOnlyInputMode;
		SetIgnoreLookInput(false);
		bShowMouseCursor = false;
		SetInputMode(GameOnlyInputMode);
	}
	else
	{
		FInputModeGameAndUI gameuionly;
		SetIgnoreLookInput(true);
		bShowMouseCursor = true;
		SetInputMode(gameuionly);
	}
}


