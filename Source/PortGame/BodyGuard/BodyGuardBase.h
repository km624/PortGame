// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BodyGuardBase.generated.h"

UENUM(BlueprintType)
enum class EBodyGuardOptionType :uint8
{
	BodyGuardPostion = 0 UMETA(DisplayName = "Pos"),
	BodyGuardLogic UMETA(DisplayName = "Logic")

};
/**
 * 
 */
UCLASS()
class PORTGAME_API UBodyGuardBase : public UObject
{
	GENERATED_BODY()

public:
	UBodyGuardBase();

	virtual void SetOption(class UAIBodyGuardComponent bodyguardcomp);
	
	/*virtual void OnHoverPriview();

	virtual void OnClickStart();*/

	virtual FVector CalculatePawnPostion(APawn* player ,int32 index, int32 arrayConut);
protected:

	UPROPERTY()
	TObjectPtr<class UAIBodyGuardComponent> BodyguardComponent;

	EBodyGuardOptionType BodyGuardOptionType = EBodyGuardOptionType::BodyGuardPostion;

	UPROPERTY()
	TSubclassOf<class ADummyPrieviewActor> PriviewActorClass;

	UPROPERTY()
	TArray<TObjectPtr<class ADummyPrieviewActor>> PriviewActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffsetX = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffsetY = -150.0f;
	
};
