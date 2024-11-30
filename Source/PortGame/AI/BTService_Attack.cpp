// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Attack.h"
#include "AIController.h"
#include "Interface/PGNPCCharacterInterface.h"
#include "PGAI.h"
#include "BehaviorTree/BlackboardComponent.h"



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

	IPGNPCCharacterInterface* AIPawn = Cast<IPGNPCCharacterInterface>(ControllingPawn);
	if (AIPawn == NULL)
	{
		return;
	}

	
	AIPawn->AttackByAI();
	
	
}
