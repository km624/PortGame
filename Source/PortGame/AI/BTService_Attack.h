// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTService_Attack : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_Attack();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
