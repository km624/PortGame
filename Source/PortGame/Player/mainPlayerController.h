// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/CharacterEnumData.h"
#include "mainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API AmainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AmainPlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* aPawn) override;

protected:
	void AllFindCharacterData();

public:
	void SetUpMainWidget();

	void SetUpSelectWidget();

protected:
	TSubclassOf<class UPGMainWidget> MainWidgetClass;

	TObjectPtr<class UPGMainWidget> MainWidget;

	TSubclassOf<class UPGSelectWidget> SelectWidgetClass;

	TObjectPtr<class UPGSelectWidget> SelectWidget;

public:
	bool SetSelectCharcterData(class UPlayerCharacterDataAsset* characterData);


protected:
	void AddSpawnLocation(FVector location,FRotator rotator);

	void SpawnCharacter(int8 num);

public:
	void ShowSelectWeaponWidget(EPlayerCharacterType characterType);
	
protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TArray<TObjectPtr<class UPlayerCharacterDataAsset>> AllPlayerDatas;

	TArray<FTransform> SpawnLocation;

	UPROPERTY(VisibleAnywhere)
	TMap<int8, class UPlayerCharacterDataAsset*> SelectPlayerDatasMap;
	UPROPERTY(VisibleAnywhere)
	TMap<int8, class APGPlayerCharacter*> SpawnCharacters;


public:
	void SetSelectWeaponrData(class UWeaponData* weaponData);

protected:
	void FindGunData();

	void FindSwordData();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class UWeaponData>> GunDatas;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class UWeaponData>> SwordDatas;


	int8 SelectNum;

public:
	void SelectComplete();

public:
	void ShowCharacterStat(class UPlayerCharacterDataAsset* characterData);

};
