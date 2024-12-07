// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_WhoTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "Interface/PGNPCCharacterInterface.h"
#include "Interface/PGAICharacterInterface.h"
#include "PGAI.h"
#include "PortGame/PortGame.h"

UBTDecorator_WhoTarget::UBTDecorator_WhoTarget()
{
	NodeName = TEXT("IsATargetPlayer");
}

bool UBTDecorator_WhoTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// Patrol , Detect¶û À¯»çÇÔ
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IPGAICharacterInterface* AIPawn = Cast<IPGAICharacterInterface>(ControllingPawn);
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
	if (Target->ActorHasTag(TAG_PLAYER))
	{
		return true;
	}


	return false;
}
