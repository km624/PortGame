// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/CharacterEnumData.h"
#include "BaseCharacterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBaseCharacterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UBaseCharacterDataAsset();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "Character")
	FName Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FString MeshName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	EPlayerCharacterType Charactertype;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TObjectPtr<class USkeletalMesh> SkeletalMesh;
	
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 TeamId;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<class UWeaponData> WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<class USkillBase> SkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UltiSkill")
	TSubclassOf<class UUltiSkill> UltiSkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	TObjectPtr<class  UNiagaraSystem> CharacterTypeEffect;
	
};
