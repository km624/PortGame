// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BodyGuard/BodyGuardBase.h"
#include "Data/AIAttackEnumData.h"
#include "BodyGuardLogic.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBodyGuardLogic : public UBodyGuardBase
{
	GENERATED_BODY()
	
public:
	UBodyGuardLogic();

	virtual void SetOption(class UAIBodyGuardComponent* bodyguardcomp, int32 optionnum, class UBGBaseOptionDataAsset* dataassset) override;

	virtual void OnClickStart() override;

protected:
	EAIAttackEnumData AttackType;


};
