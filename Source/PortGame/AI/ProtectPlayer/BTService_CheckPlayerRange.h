// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckPlayerRange.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTService_CheckPlayerRange : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckPlayerRange();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxDistance;
	
};
