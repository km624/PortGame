// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "SkillETC.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API USkillETC : public USkillBase
{
	GENERATED_BODY()
	
public:
	USkillETC();

public:
	virtual void OnSkill() override;

protected:
	void PlaySkillETCMontage();

	void EndPlaySkillETCMontage(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void EndSkill() override;
	
	

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> SkillETCMontage;

	FOnMontageEnded EndDelegate;



};
