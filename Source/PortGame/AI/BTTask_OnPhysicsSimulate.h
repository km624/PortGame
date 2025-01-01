// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_OnPhysicsSimulate.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTTask_OnPhysicsSimulate : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_OnPhysicsSimulate();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	uint8 bOnPhysics : 1;
};
