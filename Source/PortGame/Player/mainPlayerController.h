// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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

	void SetUpMainWidget();

public:
	bool SetSelectCharcterData(class UPlayerCharacterDataAsset* characterData);


protected:
	void AddSpawnLocation(FVector location,FRotator rotator);

	void SpawnCharacter(int8 num);
	

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TArray<TObjectPtr<class UPlayerCharacterDataAsset>> AllPlayerDatas;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class UPlayerCharacterDataAsset>> SelectPlayerDatas;*/

	TArray<FTransform> SpawnLocation;

	//TArray<bool> bIsSelected;

	TSubclassOf<class UPGMainWidget> MainWidgetClass;
	
	TObjectPtr<class UPGMainWidget> MainWidget;

	UPROPERTY(VisibleAnywhere)
	TMap<int8, class UPlayerCharacterDataAsset*> SelectPlayerDatasMap;
	UPROPERTY(VisibleAnywhere)
	TMap<int8, class APGPlayerCharacter*> SpawnCharacters;


};
