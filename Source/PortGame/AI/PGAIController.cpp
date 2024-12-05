// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PGAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "AI/PGAI.h"
#include "PortGame/PortGame.h"

APGAIController::APGAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBoard(TEXT("/Script/AIModule.BlackboardData'/Game/PortGame/AI/BB_PGAICharacter.BB_PGAICharacter'"));
	if (BBoard.Object)
	{
		BBAsset = BBoard.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTree(TEXT("/Script/AIModule.BehaviorTree'/Game/PortGame/AI/BT_PGBehavior.BT_PGBehavior'"));
	if (BTree.Object)
	{
		BTAsset = BTree.Object;
	}

}

void APGAIController::RunAI()
{
	SLOG(TEXT("RunAi"));
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		BlackboardComp->SetValueAsVector(BBKEY_CURRENTPOS, GetPawn()->GetActorLocation());
		//Blackboard->SetValueAsVector(BBKEY_CURRENTPOS, GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void APGAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void APGAIController::BlackBoardReset()
{
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (BlackboardComp->GetValueAsObject(BBKEY_TARGET))
	{
		APawn* pawn = Cast<APawn>(BlackboardComp->GetValueAsObject(BBKEY_TARGET));	
		if (pawn->ActorHasTag(TAG_PLAYER))
		{
			uint32 targetCount = BlackboardComp->GetValueAsInt(BBKEY_PLAYERTARGETCOUNT);
			BlackboardComp->SetValueAsInt(BBKEY_PLAYERTARGETCOUNT, --targetCount);
		}

		BlackboardComp->SetValueAsObject(BBKEY_TARGET,nullptr);
	}
	
}

void APGAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	RunAI();
}

void APGAIController::StopTree()
{
	StopAI();

}

void APGAIController::StartTree()
{
	SLOG(TEXT("StartTree"));
	RunAI();
	
}
