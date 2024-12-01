// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_TargetCircleDistance.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTDecorator_TargetCircleDistance : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_TargetCircleDistance();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
