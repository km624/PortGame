// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/CharacterEnumData.h"
#include "PGMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGMainWidget(const FObjectInitializer& ObjectInitializer);

public:
	void SetUpMainWidget();
	

protected:
	UFUNCTION()
	void SelectButtonClicked();
	UFUNCTION()
	void QuitGame();
	

protected:
	

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton>Button_SelectCharacter;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton>Button_EXIT;
	
};
