// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGProtectWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGProtectWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPGProtectWidget(const FObjectInitializer& ObjectInitializer);

public:

	void SetupMaxProtectCount(int32 max);

	void ChangeProtectButtonPadding(bool bShowOption);

	void UpdateprotectCount(int32 currentnum);


protected:
	int32 MaxCount;

	int32 CurrentCount;

protected:
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_MaxCount;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_CurrentCount;


public:
	void SetUpOptionButton(AActor* playercharacter , TArray<class UBGBaseOptionDataAsset*>& optiondataAssets);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGProtectButtonWidget> BP_ProtectOptionButton_1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGProtectButtonWidget> BP_ProtectOptionButton_2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGProtectButtonWidget> BP_ProtectOptionButton_3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPGProtectButtonWidget> BP_ProtectOptionButton_4;
	

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> PaddingAnimation;
};
