// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnForward.h"
#include "PGAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "Interface/PGNPCCharacterInterface.h"
#include "Interface/PGAICharacterInterface.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_TurnForward::UBTTask_TurnForward()
{
}

EBTNodeResult::Type UBTTask_TurnForward::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (nullptr == AIController)
	{
		return EBTNodeResult::Failed;
	}

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

	FVector CurrentLocation = ControllingPawn->GetActorLocation();

	FVector NextWaypoint = AIController->GetPathFollowingComponent()->GetCurrentTargetLocation();
	
		
	FVector Direction = NextWaypoint - CurrentLocation;
	Direction.Z = 0; // 수평 회전만 고려

	
	FRotator CharacterRot = FRotationMatrix::MakeFromX(Direction).Rotator();

		
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), CharacterRot, GetWorld()->GetDeltaSeconds(), 5.0f));
	



	return EBTNodeResult::Succeeded;
}
