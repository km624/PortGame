// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_CheckAttackRange.h"
#include "AIController.h"
//#include "Interface/PGNPCCharacterInterface.h"
#include "Interface/PGAICharacterInterface.h"
#include "PGAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PortGame/PortGame.h"

UBTTask_CheckAttackRange::UBTTask_CheckAttackRange()
{
}

EBTNodeResult::Type UBTTask_CheckAttackRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == NULL)
	{
		return EBTNodeResult::Failed;
	}

	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return EBTNodeResult::Failed;
	}

	IPGAICharacterInterface* AIPawn = Cast<IPGAICharacterInterface>(ControllingPawn);
	if (AIPawn == NULL)
	{
		return EBTNodeResult::Failed;
	}

	//Å¸°Ù °ªÀ» °¡Á®¿È (Å¸°ÙÀÇ PAawn)
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	//NPCÀÇ Å¸°Ù°ú ³ªÀÇ °Å¸®¸¦ °¡Á®¿È
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);

	float AttackRangeWithRadius = AIPawn->GetAIAttackRange(DistanceToTarget, Target);
	bool canAttack;
	canAttack = (DistanceToTarget <= AttackRangeWithRadius);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_INATTACKRANGE, canAttack);

	return EBTNodeResult::Succeeded;
}
