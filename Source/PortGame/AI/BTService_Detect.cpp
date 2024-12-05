// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "AIController.h"
#include "Interface/PGNPCCharacterInterface.h"
#include "PGAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/PGCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "PortGame/PortGame.h"
#include "GenericTeamAgentInterface.h"


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	//이미 플레이어를 타겟 했다면
	AActor* TargetPlayer = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (TargetPlayer)
	{
		if (TargetPlayer->ActorHasTag(TAG_PLAYER))
			return;
	}

	float DetectRadius = AIPawn->GetAIDetectRange();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_PGACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		
		float MinTargetDistance = FLT_MAX;
		AActor* TargetActor = NULL;
		IGenericTeamAgentInterface* MypawnTeam = Cast<IGenericTeamAgentInterface>(ControllingPawn);
		int32 TargetingCount = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKEY_PLAYERTARGETCOUNT);

		for (auto const& OverlapResult : OverlapResults)
		{
			// 다른 팀
			if (MypawnTeam->GetTeamAttitudeTowards(*OverlapResult.GetActor()))
			{
				//플레이어 타깃 카운트가 [ ] 이하라면
				if (TargetingCount < CanTargetPlayerCount)
				{
					if (OverlapResult.GetActor()->ActorHasTag(TAG_PLAYER))
					{
						TargetActor = OverlapResult.GetActor();
						
						APawn* Pawn = Cast<APawn>(TargetActor);

						OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
						TargetingCount++;
						OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_PLAYERTARGETCOUNT,TargetingCount);

						DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Purple, false, 0.2f);
						DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
						DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
						return;
					}
				}

				// 거리별로 타겟팅
				float TargetDistnace = TargetToDistance(ControllingPawn->GetActorLocation(), OverlapResult.GetActor()->GetActorLocation());
				if (TargetDistnace < MinTargetDistance)
				{
					MinTargetDistance = TargetDistnace;
					TargetActor = OverlapResult.GetActor();
					
				}
			}
		
		}
		if (TargetActor)
		{
			
			APawn* Pawn = Cast<APawn>(TargetActor);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
			DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
			DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
			DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
	
			return;
		}
		
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);


}

float UBTService_Detect::TargetToDistance(FVector myloc,FVector targetLoc)
{

	FVector MyLocation = myloc;
	FVector TargetLocation = targetLoc;
	float Distance = FVector::Dist(MyLocation, TargetLocation);
	return Distance;
}
