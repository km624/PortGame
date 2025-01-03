// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_TimerMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTTask_TimerMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
    UBTTask_TimerMoveTo();

protected:
   
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    float TimeLimit;  

    float ElapsedTime;
	
};
