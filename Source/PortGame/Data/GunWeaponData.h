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

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("GunData"), GetFName());
	}

public:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
	TObjectPtr<class UAnimMontage> ReloadMontage;*/

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Gun)
	FPGGunStat GunStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effect)
	TObjectPtr<class UNiagaraSystem> NAGunEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStat)
	uint8 bIsRifle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	TObjectPtr<class USoundBase> GunFireSFX;
};
