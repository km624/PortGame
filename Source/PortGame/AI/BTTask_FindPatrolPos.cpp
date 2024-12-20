// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"
#include "PGAI.h"
#include "AIController.h"
//#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "Interface/PGNPCCharacterInterface.h"
#include "Interface/PGAICharacterInterface.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == NULL)
	{
		return EBTNodeResult::Failed;
	}

	////버전업이 되서 V1
	//UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	//if (NavSystem == NULL)
	//{
	//	return EBTNodeResult::Failed;
	//}

	IPGAICharacterInterface* AIPawn = Cast<IPGAICharacterInterface>(ControllingPawn);
	if (AIPawn == NULL)
	{
		return EBTNodeResult::Failed;
	}

	//FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_CURRENTPOS);
	FVector Origin = ControllingPawn->GetActorLocation();
	//FNavLocation NextPatrolPos;

	//float PatrolRadius = AIPawn->GetPatrolRadius();

	AActor* myField = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_MYFIELD));
	if (myField == NULL)
	{
		return EBTNodeResult::Failed;
	}
	FVector FieldSize = myField->GetActorScale() * 50.0f;

	FVector PatrolLocation = FVector(FMath::FRandRange(-FieldSize.X, FieldSize.X), FMath::FRandRange(-FieldSize.Y, FieldSize.Y), 100.0f) + myField-> GetActorLocation();
	//FRotator SpawnRotation = FRotator(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f);

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, PatrolLocation);

	FVector Turn = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_PATROLPOS);
		float TurnSpeed = AIPawn->AITurnSpeed();

		
		FVector LookVector = Turn - ControllingPawn->GetActorLocation();
		LookVector.Z = 0.0f;
		FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

		
		ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 100.0f));
		return EBTNodeResult::Succeeded;

	//if (NavSystem->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextPatrolPos))
	//{
	//	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);

	//	FVector Turn = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_PATROLPOS);
	//	float TurnSpeed = AIPawn->AITurnSpeed();

	//	//NPC가 바라봐야 한느 회전값을 구함
	//	FVector LookVector = Turn - ControllingPawn->GetActorLocation();
	//	LookVector.Z = 0.0f;
	//	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	//	//서서히 해당 로테이션으로 지정된 속도로 돌아가게
	//	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 100.0f));
	//	return EBTNodeResult::Succeeded;
	//}




	//return EBTNodeResult::Failed;
}
