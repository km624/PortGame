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
	////HUD의 클래스 정보
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	//TSubclassOf<class UPGHudWidget> PGHudWidgetClass;

	////생성할 위젯을 담을 위젯 포인트 보관하는 변수
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	//TObjectPtr<class UPGHudWidget> PGHudWidget;

protected:
	//virtual void GunRecoilCameraShake(TSubclassOf<class UCameraShakeBase> camerashake) override;
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
	


	
};
