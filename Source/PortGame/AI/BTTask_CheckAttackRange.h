// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTTask_CheckAttackRange : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CheckAttackRange();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
