// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGSelectWeaponWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGSelectWeaponWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPGSelectWeaponWidget(const FObjectInitializer& ObjectInitializer);

	void SetUpWeapon(const TArray<class UWeaponData*>& weapondatas);

protected:
	TSubclassOf<class UPGWeaponButtonWidget> WeaponButtonClass;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> ScrollBox_Weapon;


	
};
