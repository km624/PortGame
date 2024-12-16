// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "SkillBlueArchive.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API USkillBlueArchive : public USkillBase
{
	GENERATED_BODY()

public:
	USkillBlueArchive();

	virtual void SetSkillSetting(class APGBaseCharacter* owner) override;

public:
	virtual void OnSkill() override;

protected:
	void PlaySkillgrenadeMontage();

	void EndPSkillgrenadeMontage(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void EndSkill() override;



protected:

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BlueArchive)
	TObjectPtr<class UAnimMontage> SkillBlueArchiveMontage;*/
	FOnMontageEnded EndDelegate;

	const static FString SkillMontage;

	
};
