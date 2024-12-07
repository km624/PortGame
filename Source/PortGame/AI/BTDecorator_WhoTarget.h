// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_WhoTarget.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTDecorator_WhoTarget : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_WhoTarget();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
