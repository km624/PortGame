// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PGCharacterStat.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct  FPGCharacterStat : public FTableRowBase
{

	GENERATED_BODY()

public:
	FPGCharacterStat();

	FPGCharacterStat(int32 level);
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float HitGauge;

	FPGCharacterStat operator+(const FPGCharacterStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FPGCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FPGCharacterStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}

	FPGCharacterStat& operator+=(int32 AddLevel) 
	{

		MaxHp += 100.0f* AddLevel;
		Attack += 5.0f * AddLevel;
		AttackRange += 5.0f * AddLevel;
		AttackSpeed += 0.02f * AddLevel;
		MovementSpeed += 0;
		HitGauge += 1.0f * AddLevel;

		return *this;
	}

	
	
};
