// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/BGBaseOptionDataAsset.h"
#include "Data/AIAttackEnumData.h"
#include "BGLogicOptionDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBGLogicOptionDataAsset : public UBGBaseOptionDataAsset
{
	GENERATED_BODY()

public:
	UBGLogicOptionDataAsset();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EAIAttackEnumData AttackType;

	
};
