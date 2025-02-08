// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_GameStart.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PGAI.h"
#include "PortGame/PortGame.h"
#include "Engine/LevelScriptActor.h"

bool UBTDecorator_GameStart::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);


	bool bGameStart = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_GAMESTART);
	if (bGameStart)
	{
		return true;
	}
	else
		return false;
}
