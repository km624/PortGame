// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void SetPlayerIconImage();

	void ChangeFieldColor(int8 teamid);

	void VisibleLockImage(bool lock);

	void SetNPCIconImage(int8 teamid);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_lock;
	
};
