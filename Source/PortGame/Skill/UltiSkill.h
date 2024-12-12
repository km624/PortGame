// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase.h"
#include "UltiSkill.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UUltiSkill : public USkillBase
{
	GENERATED_BODY()

public:

	UUltiSkill();

	virtual void OnSkill() override;

protected:
	void PlayUltiSkillMontage();

	void EndUltiSkilleMontage(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void EndSkill() override;

	

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BlueArchive)
	TObjectPtr<class UAnimMontage> UltiSkillMontage;

	FOnMontageEnded EndDelegate;
	
};
