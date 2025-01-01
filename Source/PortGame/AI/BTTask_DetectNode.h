// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DetectNode.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTTask_DetectNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_DetectNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	float TargetToDistance(FVector myloc, FVector targetLoc);
};
