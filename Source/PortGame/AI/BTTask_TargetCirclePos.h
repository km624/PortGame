// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TargetCirclePos.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTTask_TargetCirclePos : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_TargetCirclePos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
