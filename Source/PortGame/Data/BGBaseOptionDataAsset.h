// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BGBaseOptionDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBGBaseOptionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UBGBaseOptionDataAsset();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UBodyGuardBase> BodyGuardClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString OptionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffsetX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffsetY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BodyGuardSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 OptionBGGauge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIsBehavior : 1;



	
};
