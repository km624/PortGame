// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USkillWidget(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetUpWidget(float maxcooltime);

	UFUNCTION()
	void StartSkillCooltime(bool start);

protected:
	void UpdateSkillCooltime();

protected:

	float PreviouscooltimePercent;


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_Skill;

	UPROPERTY()
	float Skillcooltime;
	//14°­ ÇöÀç hp
	UPROPERTY()
	float Currentcooltime;

	FTimerHandle SkillCooltimeTimerHandle;

	
};
