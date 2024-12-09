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

	void OnbIsCrouch(bool crouching);

	virtual void EndSkill() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nikke")
	
	float SkillWallTime = 20.0f;

	bool IsCrouching;

	
};
