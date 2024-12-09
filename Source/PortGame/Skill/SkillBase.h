// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnbIsSkillDelegate, bool /*bIsSkill*/);
UCLASS()
class PORTGAME_API USkillBase : public UObject
{
	GENERATED_BODY()
	

public:
	USkillBase();

	void SetSkillSetting(class APGBaseCharacter* owner);

	virtual void OnSkill();

	FORCEINLINE bool GetbIsSkill() { return bIsSkill; }

public:
	FOnbIsSkillDelegate OnbIsSkill;

	FORCEINLINE float GetSkillCooltime() { return SkillCooltime; }

protected:
	void SetTimer();

	virtual void EndSkill();

protected:
	TObjectPtr<class APGBaseCharacter> ownercharacter;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillCooltime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	uint8 bIsSkill:1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	uint8  bCanSkill:1;

	FTimerHandle CooldownTimerHandle;

	
};
