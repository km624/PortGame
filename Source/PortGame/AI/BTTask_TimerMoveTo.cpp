// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TimerMoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_TimerMoveTo::UBTTask_TimerMoveTo()
{
	bNotifyTick = true; 
	TimeLimit = 5.0f;   
	ElapsedTime = 0.0f;
    NodeName = TEXT("Timer Move To");
}



void UBTTask_TimerMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    ElapsedTime += DeltaSeconds;

    if (ElapsedTime >= TimeLimit)
    {
        // 시간 초과 시 성공으로 종료
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    // 기본 Move To 상태 계속 체크
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

}
