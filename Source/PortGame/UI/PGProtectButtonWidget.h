// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGProtectButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGProtectButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupProtectButton(AActor* playerCharacter, int32 optionnum);

protected:
	UFUNCTION()
	void OnButtonClick();

	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton>  Button_protect;
	
	UPROPERTY()
	int32 OptionNum;

	UPROPERTY()
	TObjectPtr<AActor> PlayerCharacter;
};
