// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_NotVisibleAttack.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTTask_NotVisibleAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_NotVisibleAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
