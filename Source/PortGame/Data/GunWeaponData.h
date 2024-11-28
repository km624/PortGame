// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/WeaponData.h"
#include "Struct/PGGunStat.h"
#include "GunWeaponData.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UGunWeaponData : public UWeaponData
{
	GENERATED_BODY()

public:
	UGunWeaponData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
	TObjectPtr<class UAnimMontage> ReloadMontage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Gun)
	FPGGunStat GunStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;
};
