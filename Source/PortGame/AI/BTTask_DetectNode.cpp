// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_DetectNode.h"
#include "PGAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/PGAICharacterInterface.h"
#include "Field/FieldManager.h"
#include "GameLevel/PGGameLevelScriptActor.h"
#include "Engine/LevelScriptActor.h"
#include "PortGame/PortGame.h"
#include "Physics/PGCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

UBTTask_DetectNode::UBTTask_DetectNode()
{
}

EBTNodeResult::Type UBTTask_DetectNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == NULL)
	{
		return EBTNodeResult::Failed;
	}

	IPGAICharacterInterface* AIPawn = Cast<IPGAICharacterInterface>(ControllingPawn);
	if (AIPawn == NULL)
	{
		return EBTNodeResult::Failed;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();

	if (nullptr == World)
	{
		return EBTNodeResult::Failed;
	}

	float DetectRadius = AIPawn->GetAIDetectRange();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);

	bool protecteField = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_PROTECTFIELD);

	bool bResult = false;
	FVector Detectsize;
	AActor* myfield = nullptr;

	if (protecteField)
	{
		myfield = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_MYFIELD));
		if (myfield != nullptr)
		{
			Detectsize = myfield->GetActorScale() * 75.0f;
			bResult = World->OverlapMultiByChannel(
				OverlapResults,
				myfield->GetActorLocation(),
				FQuat::Identity,
				CCHANNEL_PGACTION,
				FCollisionShape::MakeBox(Detectsize),
				CollisionQueryParam
			);
		}

	}
	else
	{
		bResult = World->OverlapMultiByChannel(
			OverlapResults,
			Center,
			FQuat::Identity,
			CCHANNEL_PGACTION,
			FCollisionShape::MakeSphere(DetectRadius),
			CollisionQueryParam
		);
	}

	if (bResult)
	{
		float MinTargetDistance = FLT_MAX;
		AActor* TargetActor = NULL;
		IGenericTeamAgentInterface* MypawnTeam = Cast<IGenericTeamAgentInterface>(ControllingPawn);

		for (auto const& OverlapResult : OverlapResults)
		{
			// 다른 팀
			if (MypawnTeam->GetTeamAttitudeTowards(*OverlapResult.GetActor()))
			{
				TargetActor = OverlapResult.GetActor();


				// 거리별로 타겟팅
				float TargetDistnace = TargetToDistance(ControllingPawn->GetActorLocation(), OverlapResult.GetActor()->GetActorLocation());
				if (TargetDistnace < MinTargetDistance)
				{
					MinTargetDistance = TargetDistnace;
					TargetActor = OverlapResult.GetActor();

				}
			}

		}

		//제일 거리가 적을 애를 타겟팅 
		// ( 플레이어 관여하는 액터랑 상관없이 몇명이든 아무나)
		if (TargetActor)
		{

			APawn* Pawn = Cast<APawn>(TargetActor);

			APawn* CurrentTarget = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));

			if(Pawn!=CurrentTarget)
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);

			if (protecteField)
				DrawDebugBox(World, myfield->GetActorLocation(), Detectsize, FColor::Green, false, 0.2f);
			else
			{
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
			}


			return EBTNodeResult::Succeeded;
		}

	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

	if (protecteField)
	{
		if (myfield != nullptr)
			DrawDebugBox(World, myfield->GetActorLocation(), Detectsize, FColor::Yellow, false, 0.2f);
	}
	else
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);

	return EBTNodeResult::Succeeded;
}

float UBTTask_DetectNode::TargetToDistance(FVector myloc, FVector targetLoc)
{

	FVector MyLocation = myloc;
	FVector TargetLocation = targetLoc;
	float Distance = FVector::Dist(MyLocation, TargetLocation);
	return Distance;
}
