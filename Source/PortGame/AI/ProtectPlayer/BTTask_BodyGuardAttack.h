// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Data/AIAttackEnumData.h"
#include "BTTask_BodyGuardAttack.generated.h"
/**
 * 
 */
UCLASS()
class PORTGAME_API UBTTask_BodyGuardAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_BodyGuardAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
protected:
	UPROPERTY()
	EAIAttackEnumData AttackEnumData;
	
};
