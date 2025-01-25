// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ProtectPlayer/BTTask_TurnAttackPop.h"
#include "AI/PGAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "Interface/PGNPCCharacterInterface.h"
#include "Interface/PGAICharacterInterface.h"
#include "PortGame/PortGame.h"

UBTTask_TurnAttackPop::UBTTask_TurnAttackPop()
{
	NodeName = TEXT("AttackTurn");
}

EBTNodeResult::Type UBTTask_TurnAttackPop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	
	FRotator AttackRotator = OwnerComp.GetBlackboardComponent()->GetValueAsRotator(BBKEY_BGATTACKROT);
	
	
	
	ControllingPawn->SetActorRotation(AttackRotator);

	return EBTNodeResult::Succeeded;
}
