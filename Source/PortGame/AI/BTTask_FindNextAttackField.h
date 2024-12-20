// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindNextAttackField.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTTask_FindNextAttackField : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindNextAttackField();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
