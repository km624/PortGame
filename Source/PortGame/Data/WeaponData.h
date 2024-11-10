// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Struct/PGCharacterStat.h"
#include "Struct/PGGunStat.h"
#include "WeaponData.generated.h"

/**
 * 
 */
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ComboMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UComboData> ComboData;
	//몽타주 섹션이름

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
	TObjectPtr<class UAnimMontage> ReloadMontage;

	UPROPERTY(EditAnywhere, Category = Gun)
	FPGGunStat GunStat;

	
};
