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
        // �ð� �ʰ� �� �������� ����
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    // �⺻ Move To ���� ��� üũ
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

}
