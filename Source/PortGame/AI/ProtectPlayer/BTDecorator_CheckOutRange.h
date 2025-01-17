// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckOutRange.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTDecorator_CheckOutRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckOutRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
