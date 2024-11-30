// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTService_AttackRange : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_AttackRange();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
