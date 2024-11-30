// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterEnumData.generated.h"

UENUM(BlueprintType)
enum class EPlayerCharacterType :uint8
{
	BlueArchive = 0 UMETA(DisplayName = "BlueArchive"),
	Nikke UMETA(DisplayName = "Nikke"),
	ETC UMETA(DisplayName = "ETC")
};
