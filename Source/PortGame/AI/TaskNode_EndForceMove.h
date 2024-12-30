// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TaskNode_EndForceMove.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UTaskNode_EndForceMove : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTaskNode_EndForceMove();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
