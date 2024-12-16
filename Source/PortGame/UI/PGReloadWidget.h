// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGReloadWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGReloadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPGReloadWidget(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void NativeOnInitialized() override;


public:
	void SetUpReloadWidget(float reloadtime);

	UFUNCTION()
	void StartReloadtime(bool start , float reloadtime);

protected:
	void UpdateReloadtime();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_ReloadTime;

	UPROPERTY()
	float ReloadTime;
	//14°­ ÇöÀç hp
	UPROPERTY()
	float CurrentTime;

	FTimerHandle ReloadTimeTimerHandle;
	
};
