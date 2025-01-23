// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIAttackEnumData.generated.h"

UENUM(BlueprintType)
enum class EAIAttackEnumData :uint8
{
	NormalAttack = 0 UMETA(DisplayName = "Normal"),
	SkillAttack UMETA(DisplayName = "Skill"),
	UlitAttack UMETA(DisplayName = "Ulti")
};

