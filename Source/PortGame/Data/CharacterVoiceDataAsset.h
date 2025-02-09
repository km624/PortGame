// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/CharacterVoiceEnumData.h"
#include "CharacterVoiceDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UCharacterVoiceDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCharacterVoiceDataAsset();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Priority")
	TMap<ECharacterVoiceType, int32> VoicePriorities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync")
	TArray<TSoftObjectPtr<class USoundBase>> AttackVoices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync")
	TArray<TSoftObjectPtr<class USoundBase>> DamageVoices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Async")
	TArray<TSoftObjectPtr<class USoundBase>> TakeFieldVoices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Async")
	TArray<TSoftObjectPtr<class USoundBase>> KillEliteVoices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Async")
	TSoftObjectPtr<class USoundBase> StartVoice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Async")
	TSoftObjectPtr<class USoundBase> SkillVoice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Async")
	TSoftObjectPtr<class USoundBase> ChangeVoice;

	TSoftObjectPtr<USoundBase> GetVoiceByType(ECharacterVoiceType VoiceType);

	int32 GetVoicePriority(ECharacterVoiceType VoiceType);
};
