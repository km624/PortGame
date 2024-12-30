// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TaskNode_EndForceMove.h"
#include "PGAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UTaskNode_EndForceMove::UTaskNode_EndForceMove()
{

}

EBTNodeResult::Type UTaskNode_EndForceMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == NULL)
	{
		return EBTNodeResult::Failed;
	}


	
	OwnerComp.GetBlackboardComponent()->ClearValue(BBKEY_FORCEMOVEVECTOR);

	
	return EBTNodeResult::Succeeded;

	
}
