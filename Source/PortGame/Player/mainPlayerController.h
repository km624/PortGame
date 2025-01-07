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

protected:
	void AllFindCharacterData();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TArray<TObjectPtr<class UPlayerCharacterDataAsset>>  AllPlayerDatas;
	TArray<TObjectPtr<class UBaseCharacterDataAsset>>  AllBaseDatas;


};
