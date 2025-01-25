// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnAttackPop.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTTask_TurnAttackPop : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_TurnAttackPop();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
