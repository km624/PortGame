// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ComboData.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UComboData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UComboData();

	//몽타주 섹션이름
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionName;

	//몇개의 콤보 액션
	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	//프레임 기준 재생 속도 지정
	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;
	
};
