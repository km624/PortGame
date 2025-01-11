// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Attack.h"
#include "AIController.h"
//#include "Interface/PGNPCCharacterInterface.h"
#include "Interface/PGAICharacterInterface.h"
#include "PGAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GenericTeamAgentInterface.h"



UBTService_Attack::UBTService_Attack()
{
	NodeName = TEXT("Attack");
	Interval = 1.5f;
}

void UBTService_Attack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == NULL)
	{
		return;
	}
	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	IPGAICharacterInterface* AIPawn = Cast<IPGAICharacterInterface>(ControllingPawn);
	if (AIPawn == NULL)
	{
		return;
	}

	//Å¸°Ù °ªÀ» °¡Á®¿È (Å¸°ÙÀÇ PAawn)
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));


	if (nullptr == Target)
	{
		return;
	}
	IGenericTeamAgentInterface* team = Cast<IGenericTeamAgentInterface>(Target);
	if (team)
	{
		if (!team->GetTeamAttitudeTowards(*ControllingPawn))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

			return;
		}
	}


	IPGAICharacterInterface* TargetPawn = Cast<IPGAICharacterInterface>(Target);
	if (TargetPawn == NULL)
	{
		return;
	}



	if (TargetPawn->CheckTargetDead())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

		return;
	}
	


	//NPCÀÇ Å¸°Ù°ú ³ªÀÇ °Å¸®¸¦ °¡Á®¿È
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);

	float AttackRangeWithRadius = AIPawn->GetAIAttackRange(DistanceToTarget, Target);
	

	if (DistanceToTarget > AttackRangeWithRadius)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

		return;
	}


	if (AttackRangeWithRadius > 200.0f)
	{
		AIPawn->ShotByAI();
	}
	else
	{
		AIPawn->AttackByAI();
	}
	
	
}
