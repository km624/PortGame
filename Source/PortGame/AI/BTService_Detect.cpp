// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "AIController.h"
//#include "Interface/PGNPCCharacterInterface.h"
#include "Interface/PGAICharacterInterface.h"
#include "PGAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/PGCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "PortGame/PortGame.h"
#include "GenericTeamAgentInterface.h"
#include "Interface/AITargetPlayerInterface.h"
//#include "Character/PGBaseCharacter.h"


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

	IPGAICharacterInterface* AIPawn = Cast<IPGAICharacterInterface>(ControllingPawn);
	if (AIPawn == NULL)
	{
		return;
	}
	
	float DetectRadius = AIPawn->GetAIDetectRange();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	
	bool protecteField = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_PROTECTFIELD);

	bool bResult =false;
	FVector Detectsize;
	AActor* myfield =nullptr;

	if (protecteField)
	{
		 myfield = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_MYFIELD));
		if (myfield !=nullptr)
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
		//int32 TargetingCount = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKEY_PLAYERTARGETCOUNT);

		for (auto const& OverlapResult : OverlapResults)
		{
			// 다른 팀
			if (MypawnTeam->GetTeamAttitudeTowards(*OverlapResult.GetActor()))
			{
				TargetActor = OverlapResult.GetActor();
				/*if (OverlapResult.GetActor()->ActorHasTag(TAG_PLAYER))
				{

					APawn* TargetPawn = Cast<APawn>(TargetActor);
					IAITargetPlayerInterface* player = Cast<IAITargetPlayerInterface>(TargetPawn);

					if (player->CanPlayerTarget(ControllingPawn))
					{
						player->SetPlayerTargetPawn(ControllingPawn);
						OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, TargetPawn);
						
						if(protecteField)
							DrawDebugBox(World, myfield->GetActorLocation(), Detectsize, FColor::Purple, false, 0.2f);
						else
						{
							DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Purple, false, 0.2f);
							DrawDebugPoint(World, TargetPawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
							DrawDebugLine(World, ControllingPawn->GetActorLocation(), TargetPawn->GetActorLocation(), FColor::Green, false, 0.27f);
						}
					
						return;
					}
					
				
		
				}
				*/

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

			if (Pawn != CurrentTarget)
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
			
			if (protecteField)
				DrawDebugBox(World, myfield->GetActorLocation(), Detectsize, FColor::Green, false, 0.2f);
			else
			{
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
			}
		
	
			return;
		}
		
	}
	
	//감지가 되지 않았을때
	//만약 플레이어를 감지하고 있었다가 감지 못하면
	AActor* TargetPlayer = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (TargetPlayer)
	{
		if (TargetPlayer->ActorHasTag(TAG_PLAYER))
		{
			IAITargetPlayerInterface* player = Cast<IAITargetPlayerInterface>(TargetPlayer);
			player->DeletePlayerTargetPawn(ControllingPawn);
		}
	}
	/*APGBaseCharacter* basecharacter = Cast<APGBaseCharacter>(ControllingPawn);
	if (basecharacter)
	{
		basecharacter->SetbIsAim(false);
		basecharacter->SetbIsShoot(false);
	}*/

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);


	if (protecteField)
	{
		if(myfield!=nullptr)
			DrawDebugBox(World, myfield->GetActorLocation(), Detectsize, FColor::Yellow, false, 0.2f);
	}
	else
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	

}

float UBTService_Detect::TargetToDistance(FVector myloc,FVector targetLoc)
{

	FVector MyLocation = myloc;
	FVector TargetLocation = targetLoc;
	float Distance = FVector::Dist(MyLocation, TargetLocation);
	return Distance;
}


