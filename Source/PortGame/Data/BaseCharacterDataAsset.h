// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/CharacterEnumData.h"
#include "BaseCharacterDataAsset.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMeshNameOption : uint8
{
	ManneQuin UMETA(DisplayName = "ManneQuin"),
	Mika UMETA(DisplayName = "Mika"),
	Rapi UMETA(DisplayName = "Rapi"),
	Rover UMETA(DisplayName = "Rover"),
	MikaV2 UMETA(DisplayName = "MikaV2"),
	RapiV2 UMETA(DisplayName = "RapiV2"),
	RoverV2 UMETA(DisplayName = "RoverV2"),
	Aru UMETA(DisplayName = "Aru"),
	Alice UMETA(DisplayName = "Alice"),
	Ellen UMETA(DisplayName = "Ellen")
	
};

UCLASS()
class PORTGAME_API UBaseCharacterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UBaseCharacterDataAsset();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("BaseCharacterDataAsset"), GetFName());
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "Character")
	EMeshNameOption MeshName;

	UFUNCTION(BlueprintCallable, Category = "Combo")
	FString GetMeshNameAsString() const
	{
		switch (MeshName)
		{
		case EMeshNameOption::ManneQuin:
			return TEXT("ManneQuin");
		case EMeshNameOption::Mika:
			return TEXT("Mika");
		case EMeshNameOption::Rapi:
			return TEXT("Rapi");
		case EMeshNameOption::Rover:
			return TEXT("Rover");
		case EMeshNameOption::MikaV2:
			return TEXT("MikaV2");
		case EMeshNameOption::RapiV2:
			return TEXT("RapiV2");
		case EMeshNameOption::RoverV2:
			return TEXT("RoverV2");
		case EMeshNameOption::Aru:
			return TEXT("Aru");
		case EMeshNameOption::Alice:
			return TEXT("Alice");
		case EMeshNameOption::Ellen:
			return TEXT("Ellen");
		default:
			return TEXT("");
		}
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	EPlayerCharacterType Charactertype;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TObjectPtr<class USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSubclassOf<class UAnimInstance> AnimInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<class UWeaponData> WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<class USkillBase> SkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UltiSkill")
	TSubclassOf<class UUltiSkill> UltiSkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	TObjectPtr<class  UNiagaraSystem> CharacterTypeEffect;
	
};
