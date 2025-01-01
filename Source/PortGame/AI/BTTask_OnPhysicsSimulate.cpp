// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_OnPhysicsSimulate.h"
#include "AIController.h"
//#include "Interface/PGNPCCharacterInterface.h"
#include "Interface/PGAICharacterInterface.h"
#include "PGAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PortGame/PortGame.h"



UBTTask_OnPhysicsSimulate::UBTTask_OnPhysicsSimulate()
{
	bOnPhysics = true;
}

EBTNodeResult::Type UBTTask_OnPhysicsSimulate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == NULL)
	{
		return EBTNodeResult::Failed;
	}

	IPGAICharacterInterface* AIPawn = Cast<IPGAICharacterInterface>(ControllingPawn);
	if (AIPawn == NULL)
	{
		return EBTNodeResult::Failed;
	}
	

	return EBTNodeResult::Succeeded;
}
