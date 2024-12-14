// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PGGunStat.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPGGunStat : public FTableRowBase
{

	GENERATED_BODY()

public:
	FPGGunStat();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStat)
	int32 ammoMaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStat)
	float reloadingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStat)
	float ShootInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GunStat)
	float traceDistance;


};


