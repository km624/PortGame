// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterVoiceEnumData.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class ECharacterVoiceType : uint8
{
    Attack       UMETA(DisplayName = "Attack"),
    Damage       UMETA(DisplayName = "Damage"),
    TakeField    UMETA(DisplayName = "Take Field"),
    KillElite    UMETA(DisplayName = "Kill Elite"),
    Start        UMETA(DisplayName = "Start"),
    Skill        UMETA(DisplayName = "Skill"),
    Change       UMETA(DisplayName = "Change")
};

//static int32 GetPriority(ECharacterVoiceType VoiceType)
//{
//    switch (VoiceType)
//    {
//    case ECharacterVoiceType::Attack:     return 1;
//    case ECharacterVoiceType::Damage:     return 2;
//    case ECharacterVoiceType::Skill:      return 3;
//    case ECharacterVoiceType::TakeField:  return 5;
//    case ECharacterVoiceType::KillElite:  return 4;
//    case ECharacterVoiceType::Change:     return 5;
//    case ECharacterVoiceType::Start:      return 1; 
//    default:                              return 0;
//    }
//}
