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

#include "Engine/LevelScriptActor.h"
#include "Interface/LevelGameStartInterface.h"
#include "UI/StartCountWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Data/CharacterVoiceEnumData.h"


APGPlayerController::APGPlayerController()
{
	static ConstructorHelpers::FClassFinder<UStartCountWidget> startcountclass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/BP_StartWidget.BP_StartWidget_C'"));
	if (startcountclass.Class)
	{
		StartCountWidgetClass = startcountclass.Class;
	}

	BGMComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComponent"));
	BGMComponent->bIsUISound = true;  
	BGMComponent->SetVolumeMultiplier(0.2f);
	//BGMComponent->RegisterComponent();

	bGameStart = false;
	
}

void APGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	

}

void APGPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(ChangedCharacterTimerHanlde);
	GetWorld()->GetTimerManager().ClearTimer(BGMTimerHandle);
}

void APGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

}

void APGPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
	APGPlayerCharacter* currentplayer = Cast<APGPlayerCharacter>(aPawn);
	
	ChangeBGM(currentplayer);

	SetViewTarget(GetPawn());

	currentplayer->SetCharacterInputData(EControlData::Base);
	
	if (currentplayer)
	{
		currentplayer->HiddenWidget();
		if (bGameStart)
		{
			currentplayer->HudWidgetAddviewport();
			currentplayer->PlayCharacterVoice(ECharacterVoiceType::Change);
		}
		else
		{
			currentplayer->PlayCharacterVoice(ECharacterVoiceType::Start);
		}
			
	}

}

void APGPlayerController::ShowResolutionSetting()
{
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
}

void APGPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	CreateGameStartCountWidget();
	BindGameStart();

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
	UpdateGameEnd();
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
	UpdateGameEnd();
	for (APGPlayerCharacter* playerCharacter : PlayerCharacters)
	{
		if(!playerCharacter->GetbIsDead())
			playerCharacter->UpdateGameState(false);
	}
		SavesaveGameFile();
	
}

void APGPlayerController::UpdateGameEnd()
{
	ILevelGameStartInterface* levelgamestart = Cast<ILevelGameStartInterface>(GetWorld()->GetLevelScriptActor());
	if (levelgamestart)
	{
		levelgamestart->GameEnd();

	}
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
		//FInputModeUIOnly uionly;
		SetIgnoreLookInput(true);
		bShowMouseCursor = true;
		SetInputMode(gameuionly);
	}
}

void APGPlayerController::BindGameStart()
{
	ILevelGameStartInterface* levelgamestart = Cast<ILevelGameStartInterface>(GetWorld()->GetLevelScriptActor());
	if (levelgamestart)
	{
		
		levelgamestart->SetGameStartPlayer(this);
		SetGameStart(levelgamestart->GetbGameStart());

		for (APGAIController* aicontroller : AIPlayerControllers)
		{
			levelgamestart->SetGameStartAI(aicontroller);
		}
		
		levelgamestart->SetGameStartTimer();

	}
}


void APGPlayerController::CreateGameStartCountWidget()
{
	
	if (StartCountWidgetClass)
	{
		StartCountWidget = CreateWidget<UStartCountWidget>(this, StartCountWidgetClass);
		if (StartCountWidget)
		{
			
			StartCountWidget->AddToViewport();
			
		}
	}
	
}

void APGPlayerController::SetGameStart(bool bisGameStart)
{
	if (GetPawn())
	{
		APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(GetPawn());
		if (playerCharacter)
		{
			bGameStart = bisGameStart;
			if (bGameStart)
			{
				
				playerCharacter->HudWidgetAddviewport();
				if (StartCountWidget)
				{
					
							
					StartCountWidget->RemoveFromParent();
					StartCountWidget = NULL;
					
				}
				playerCharacter->EnableInput(this);

			}
			else
			{
				
				//playerCharacter->RemoveHudWidget();
				playerCharacter->DisableInput(this);
				
			}
		}
	}
	
	
}

void APGPlayerController::UpdateStartCount(int32 count)
{
	if (StartCountWidget)
	{
		
		StartCountWidget->UpdateTextCount(count);
	}
}

void APGPlayerController::SetBGM(APGPlayerCharacter* playercharacter)
{
	if (GetPawn())
	{
		APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(GetPawn());
		if (playerCharacter)
		{
			USoundBase* characterbgm =  playerCharacter->GetCharacterBGM();
			if (characterbgm)
			{
				if (BGMComponent->GetSound() != characterbgm)
				{
					BGMComponent->SetSound(characterbgm);
				}

			}
		}
	}

}

void APGPlayerController::ChangeBGM(APGPlayerCharacter* playercharacter)
{
	if (BGMComponent->IsPlaying())
	{
		
		float CurrentPlaybackTime = GetBGMPlaybackTime();

		BGMComponent->FadeOut(BGMFadeOut, 0.0f); 
		GetWorld()->GetTimerManager().SetTimer(BGMTimerHandle, [this, playercharacter, CurrentPlaybackTime]()
			{
				SetBGM(playercharacter);
				BGMComponent->FadeIn(BGMFadeIn, 1.0f);
				BGMComponent->Play(CurrentPlaybackTime);

				StartBGMTime = GetWorld()->GetTimeSeconds() - CurrentPlaybackTime;

			}, BGMFadeOut, false);
	}
	else
	{
		
		SetBGM(playercharacter);
		BGMComponent->Play();
		StartBGMTime = GetWorld()->GetTimeSeconds();
		
	}
	
	
}

float APGPlayerController::GetBGMPlaybackTime()
{
	if (!BGMComponent || !BGMComponent->IsPlaying() || !BGMComponent->Sound)
		return 0.0f;

	float BGMTime = GetWorld()->GetTimeSeconds() - StartBGMTime;
	float BGMDuration = BGMComponent->Sound->GetDuration(); 

	if (BGMDuration > 0.0f)
	{
		BGMTime = FMath::Fmod(BGMTime, BGMDuration); 
	}

	return BGMTime;
}


