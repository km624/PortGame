// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AIISProtected.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PGAI.h"
#include "PortGame/PortGame.h"

UBTDecorator_AIISProtected::UBTDecorator_AIISProtected()
{
	NodeName = TEXT("IsFieldProtected");
}

bool UBTDecorator_AIISProtected::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	
	bool Fiedlprotected = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_PROTECTFIELD);
	if (Fiedlprotected)
	{
		return true;
	}
	else
	{
		return false;
	}


	return false;
}
