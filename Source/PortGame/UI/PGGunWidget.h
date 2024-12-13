// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGGunWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGGunWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGGunWidget(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetUpGunWidget(int32 maxammo);

	UFUNCTION()
	void UpdateGunAmmo(int32 currentammo);

protected:
	void SmoothingAmmo(float deltatime);

protected:

	float PreviousAmmoPercent;


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URadialSlider> RadialSlider_GunAmmo;

	UPROPERTY()
	int32 MaxAmmo;

	
	UPROPERTY()
	int32 CurrentAmmo;
	
};
