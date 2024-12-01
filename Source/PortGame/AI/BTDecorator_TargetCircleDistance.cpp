// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_TargetCircleDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/PGNPCCharacterInterface.h"
#include "PGAI.h"

UBTDecorator_TargetCircleDistance::UBTDecorator_TargetCircleDistance()
{
	NodeName = TEXT("TargetCircleDistance");
}

bool UBTDecorator_TargetCircleDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// Patrol , Detect¶û À¯»çÇÔ
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IPGNPCCharacterInterface* AIPawn = Cast<IPGNPCCharacterInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	//Å¸°Ù °ªÀ» °¡Á®¿È (Å¸°ÙÀÇ PAawn)
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return false;
	}

	//NPCÀÇ Å¸°Ù°ú ³ªÀÇ °Å¸®¸¦ °¡Á®¿È
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);

	
	return bResult;
}
