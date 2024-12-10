// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "SkillNikke.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API USkillNikke : public USkillBase
{
	GENERATED_BODY()

public:
	USkillNikke();

public:
	virtual void OnSkill() override;

protected:
	void PlaySkillNikkeMontage();

	void SpawnNikkeWaill();

	void EndPSkillNikkeMontage(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void EndSkill() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nikke")
	float SkillWallTime = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nikke")
	TSubclassOf<class ANikkeWall> NikkeWallClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BlueArchive)
	TObjectPtr<class UAnimMontage> SkillNikkeMontage;

	FOnMontageEnded EndDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nikke")
	float SpawnDelayTime = 2.0f;

	FTimerHandle SpawnWallTimerHandle;
	
};
