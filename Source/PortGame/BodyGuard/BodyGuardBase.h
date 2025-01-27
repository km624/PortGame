// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BodyGuardBase.generated.h"


/**
 * 
 */
UCLASS()
class PORTGAME_API UBodyGuardBase : public UObject
{
	GENERATED_BODY()

public:
	UBodyGuardBase();

	virtual void SetOption(class UAIBodyGuardComponent* bodyguardcomp,int32 optionnum , class UBGBaseOptionDataAsset* dataassset);
	
	virtual float GetBodyGuardSpeed();

	//button에서 onclick하면 여기 inclickstart가 호출되고 bodyguardcomp의 값을 변경
	//virtual void OnHoverPriview();

	virtual void OnClickStart();

	virtual FVector CalculatePawnPostion(AActor* player ,int32 index, int32 arrayConut);
protected:

	UPROPERTY()
	TObjectPtr<class UAIBodyGuardComponent> BodyguardComponent;

	/*UPROPERTY()
	TSubclassOf<class ADummyPrieviewActor> PriviewActorClass;*/

	UPROPERTY()
	TArray<TObjectPtr<class ADummyPrieviewActor>> PriviewActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffsetX = -200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffsetY = -150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BodyGuardSpeed = 400.0f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 BodyGuardOptionNum;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 BGGaugeOption;

//protected:
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//	TObjectPtr<class UBGBaseOptionDataAsset> BodyGuardOpotionDataAsset;
	
};
