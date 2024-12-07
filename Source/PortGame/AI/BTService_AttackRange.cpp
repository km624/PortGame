// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_AttackRange.h"
#include "AIController.h"
//#include "Interface/PGNPCCharacterInterface.h"
#include "Interface/PGAICharacterInterface.h"
#include "PGAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_AttackRange::UBTService_AttackRange()
{
	NodeName = TEXT("DetectAttackRange");
	Interval = 0.5f;
}

void UBTService_AttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	//NPCÀÇ Å¸°Ù°ú ³ªÀÇ °Å¸®¸¦ °¡Á®¿È
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);

	float AttackRangeWithRadius = AIPawn->GetAIAttackRange(DistanceToTarget, Target);
	bool canAttack;
	canAttack = (DistanceToTarget <= AttackRangeWithRadius);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_INATTACKRANGE, canAttack);
}
