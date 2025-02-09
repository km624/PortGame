// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/CharacterVoiceDataAsset.h"

UCharacterVoiceDataAsset::UCharacterVoiceDataAsset()
{
    // 기본 우선순위 값 설정
    VoicePriorities.Add(ECharacterVoiceType::Attack, 1);
    VoicePriorities.Add(ECharacterVoiceType::Damage, 2);
    VoicePriorities.Add(ECharacterVoiceType::Skill, 3);
    VoicePriorities.Add(ECharacterVoiceType::TakeField, 5);
    VoicePriorities.Add(ECharacterVoiceType::KillElite, 4);
    VoicePriorities.Add(ECharacterVoiceType::Change, 6);
    VoicePriorities.Add(ECharacterVoiceType::Start, 1);
}

TSoftObjectPtr<USoundBase> UCharacterVoiceDataAsset::GetVoiceByType(ECharacterVoiceType VoiceType)
{

    switch (VoiceType)
    {
    case ECharacterVoiceType::Attack:
        return AttackVoices.Num() > 0 ? AttackVoices[FMath::RandRange(0, AttackVoices.Num() - 1)] : nullptr;

    case ECharacterVoiceType::Damage:
        return DamageVoices.Num() > 0 ? DamageVoices[FMath::RandRange(0, DamageVoices.Num() - 1)] : nullptr;

    case ECharacterVoiceType::TakeField:
        return TakeFieldVoices.Num() > 0 ? TakeFieldVoices[FMath::RandRange(0, TakeFieldVoices.Num() - 1)] : nullptr;

    case ECharacterVoiceType::KillElite:
        return KillEliteVoices.Num() > 0 ? KillEliteVoices[FMath::RandRange(0, KillEliteVoices.Num() - 1)] : nullptr;

    case ECharacterVoiceType::Start:
        return StartVoice;

    case ECharacterVoiceType::Skill:
        return SkillVoice;

    case ECharacterVoiceType::Change:
        return ChangeVoice;

    default:
        return nullptr;
    }
}

int32 UCharacterVoiceDataAsset::GetVoicePriority(ECharacterVoiceType VoiceType)
{
    if (VoicePriorities.Contains(VoiceType))
    {
        return VoicePriorities[VoiceType]; 
    }
    return 0;
}
