// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_NotVisibleAttack.h"
#include "PGAI.h"
#include "AIController.h"
//#include "Interface/PGNPCCharacterInterface.h"
#include "Interface/PGAICharacterInterface.h"
#include "PortGame/PortGame.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_NotVisibleAttack::UBTTask_NotVisibleAttack()
{
}

EBTNodeResult::Type UBTTask_NotVisibleAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);


	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}


	IPGAICharacterInterface* AIPawn = Cast<IPGAICharacterInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	if (AIPawn->CheckTargetDead(Target))
	{
		return EBTNodeResult::Failed;
	}

	AIPawn->NotVisibleAttack(Target);

	return EBTNodeResult::Succeeded;

}
