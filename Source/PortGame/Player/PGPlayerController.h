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
	////HUD�� Ŭ���� ����
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	//TSubclassOf<class UPGHudWidget> PGHudWidgetClass;

	////������ ������ ���� ���� ����Ʈ �����ϴ� ����
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	//TObjectPtr<class UPGHudWidget> PGHudWidget;

protected:
	//virtual void GunRecoilCameraShake(TSubclassOf<class UCameraShakeBase> camerashake) override;
	virtual void PlayCameraShake(TSubclassOf<class UCameraShakeBase> camerashake) override;

	//ĳ���� ����
public:
	void ChangedCharacterPossess(int8 playernum);
	

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<TObjectPtr<class APGPlayerCharacter>> PlayerCharacters;
	
};
