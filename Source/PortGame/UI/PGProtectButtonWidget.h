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
	UPGProtectButtonWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized();

	void SetupProtectButton(AActor* playerCharacter, int32 optionnum, uint8 optionGauge, FString optionName, bool bislogic);

	void SetChangeButtonStyle();

protected:
	UFUNCTION()
	void OnButtonClick();

	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton>  Button_protect;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_protect;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr < class UHorizontalBox> HorizontalBox_BGImages;

	UPROPERTY()
	TSubclassOf<UUserWidget> BGImageClass;
	
	UPROPERTY()
	int32 OptionNum;

	UPROPERTY()
	uint8 OptionGauge;

	UPROPERTY()
	TObjectPtr<AActor> PlayerCharacter;
};
