// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Struct/PGCharacterStat.h"
#include "WeaponData.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EComboMontageOptions : uint8
{
	SwordComboMontage UMETA(DisplayName = "SwordComboMontage"),
	RifleComboMontage UMETA(DisplayName = "RifleComboMontage"),
	PistolComboMontage UMETA(DisplayName = "PistolComboMontage")
};

UCLASS()
class PORTGAME_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UWeaponData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<UStaticMesh> WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FName WeaponSocket;

	UPROPERTY(EditAnywhere, Category = Weapon)
	FPGCharacterStat ModifierStat;


public:
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UAnimMontage> ComboMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UComboData> ComboData;
	//몽타주 섹션이름

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combo")
	EComboMontageOptions ComboMontageOption;

public:
	UFUNCTION(BlueprintCallable, Category = "Combo")
	FString GetComboMontageAsString() const
	{
		switch (ComboMontageOption)
		{
		case EComboMontageOptions::SwordComboMontage:
			return TEXT("SwordComboMontage");
		case EComboMontageOptions::RifleComboMontage:
			return TEXT("RifleComboMontage");
		case EComboMontageOptions::PistolComboMontage:
			return TEXT("PistolComboMontage");
		default:
			return TEXT("");
		}
	}


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effect)
	TObjectPtr<class UNiagaraSystem> NAWeaponEffect;




	
};
