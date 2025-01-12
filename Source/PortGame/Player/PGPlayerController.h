// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interface/PlayerCameraShakeInterface.h"
#include "PGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API APGPlayerController : public APlayerController, public IPlayerCameraShakeInterface
{
	GENERATED_BODY()

public:
	APGPlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* aPawn) override;


protected:
	
	virtual void PlayCameraShake(TSubclassOf<class UCameraShakeBase> camerashake) override;

	//캐릭터 변경  - Character에서 접근
public:
	void ChangedCharacterPossess(int8 playernum);
	
	//StartField에서 접근
public:
	void SpawnCharacterAdd(class APGPlayerCharacter* character , class APGAIController* aicontroller);

	void SetupAllCharcterWidget();

protected:
	void ChangeCharacterController(class APGPlayerCharacter* newcharacter, class APGPlayerCharacter* oldcharacter);

public:
	void ChangeMiniMap(bool bIsMiniMap);

	//플레이어 교체 캐릭터들 모음
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<TObjectPtr<class APGPlayerCharacter>> PlayerCharacters;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class APGAIController>> AIPlayerControllers;

protected:
	FTimerHandle ChangedCharacterTimerHanlde;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float ChangeCooltime=10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 biscooltime:1;

	//세이브 파일

public:
	class UPGSaveGame* LoadSaveFile();

	void SavesaveGameFile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPGSaveGame> SaveGameInstance;

public:
	void GameClear();

	void GameOver();

	


	
};
