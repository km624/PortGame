// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGGameStateWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGGameStateWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeOnInitialized() override;
	
	void UpdateGameState(bool bIsclear);

protected:
	UFUNCTION()
	void GoMainScreen();
	

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock>TextBlock_GameClear;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock>TextBlock_GAmeOver;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Main;


	
};
