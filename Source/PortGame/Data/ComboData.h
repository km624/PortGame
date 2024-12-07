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

	//��Ÿ�� �����̸�
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionName;

	//��� �޺� �׼�
	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	
};
