// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ProtectPlayer/MyBTService.h"
#include "AIController.h"
#include "Interface/PGAICharacterInterface.h"
#include "AI/PGAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/PGCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "PortGame/PortGame.h"
#include "GenericTeamAgentInterface.h"
//#include "Character/PGPlayerCharacter.h"
#include "Field/PGField.h"
#include "Interface/AIBodyGuardInterface.h"



//#include "Character/PGBaseCharacter.h"


UMyBTService::UMyBTService()
{
	NodeName = TEXT("DetectPtrotectPlayer");
	Interval = 1.0f;
	DetectRange = 1000.0f;
}

void UMyBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PROTECTTARGET))
	{
		return;
	}
	

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == NULL)
	{
		return;
	}

	if (ControllingPawn->ActorHasTag(TAG_PLAYER))
	{
		return;
	}

	IGenericTeamAgentInterface* MypawnTeam = Cast<IGenericTeamAgentInterface>(ControllingPawn);
	if (MypawnTeam->GetGenericTeamId() != 1)
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

	//float DetectRadius = DetectRange;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(ProtectDetect), false, ControllingPawn);



	bool bResult = false;
	FVector Detectsize;


	bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_PGACTION,
		FCollisionShape::MakeSphere(DetectRange),
		CollisionQueryParam
	);


	if (bResult)
	{

		AActor* TargetActor = NULL;
		
		

		for (auto const& OverlapResult : OverlapResults)
		{
			// 플레이어 같은팀
			if (!MypawnTeam->GetTeamAttitudeTowards(*OverlapResult.GetActor()))
			{
				TargetActor = OverlapResult.GetActor();

				if (TargetActor)
				{

					//APGPlayerCharacter* player = Cast<APGPlayerCharacter>(TargetActor);
					IAIBodyGuardInterface* player = Cast<IAIBodyGuardInterface>(TargetActor);

					if (player)
					{
						//SLOG(TEXT("PlayerDetect : %s"), *TargetActor->GetActorNameOrLabel());
						if (player->CanPlayerProtect(ControllingPawn))
						{
							APawn* playerPawn = Cast<APawn>(TargetActor);

							//죽었을때 배열에서 삭제할 델리게이트 바인딩
							AIPawn->BindDeadProtectTarget(playerPawn);

							if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_PROTECTFIELD))
							{
								
								APGField* myfield = Cast<APGField>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_MYFIELD));
								if (myfield)
								{
									if (!myfield->DeleteProtectAI(ControllingPawn))
									{
										SLOG(TEXT("%s : Fail Delete field Array"), *ControllingPawn->GetActorNameOrLabel());
										return;
									}
									OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_PROTECTFIELD, false);
								}
							}
							AActor* posactor = player->SetPlayerProtectPawn(ControllingPawn);

							if (!posactor)
							{
								SLOG(TEXT("PosActor Failed(AIServiece)"));
								return;
							}
							
							

							OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_PROTECTPOSACTOR, posactor);

							OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_PROTECTTARGET, playerPawn);

							OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_OUTPROTECTRANGE, false);
							
							return;
						}
					}
		
					
				}
			}
		}
	}


	//디버그
	//DrawDebugSphere(World, Center, DetectRange, 16, FColor::Purple, false, 0.2f);

	return;
}

