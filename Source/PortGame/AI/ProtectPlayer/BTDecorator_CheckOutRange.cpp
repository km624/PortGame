// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ProtectPlayer/BTDecorator_CheckOutRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/PGAI.h"




UBTDecorator_CheckOutRange::UBTDecorator_CheckOutRange()
{

}

bool UBTDecorator_CheckOutRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// Patrol , Detect¶û À¯»çÇÔ
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}


	
	bool outrange = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_OUTPROTECTRANGE);
	
	return outrange;
}
