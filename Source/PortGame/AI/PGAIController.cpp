// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PGAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "AI/PGAI.h"
#include "PortGame/PortGame.h"
#include "Interface/AITargetPlayerInterface.h"
#include "Field/PGField.h"


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

void APGAIController::SetMyFieldData(APGField* field)
{
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		BlackboardComp->SetValueAsObject(BBKEY_MYFIELD, field);
		BlackboardComp->SetValueAsBool(BBKEY_PROTECTFIELD, true);
	}
	
}

void APGAIController::SetAttackAIData()
{
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		BlackboardComp->SetValueAsBool(BBKEY_PROTECTFIELD, false);
	}
}

void APGAIController::RunAI()
{
	//SLOG(TEXT("RunAi"));
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void APGAIController::TOMyFieldDead(int8 teamid)
{
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	bool protectfield = BlackboardComp->GetValueAsBool(BBKEY_PROTECTFIELD);
	if (protectfield)
	{
		APGField* myfield = Cast<APGField>(BlackboardComp->GetValueAsObject(BBKEY_MYFIELD));
		if (myfield)
		{
			if(GetPawn())
				myfield->DamageFieldGauge(GetPawn(),teamid);
		}
	}

}

void APGAIController::AIIsDead()
{
	StopMovement();
	BlackBoardReset();
	StopAI();
	
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
			
			IAITargetPlayerInterface* player = Cast<IAITargetPlayerInterface>(pawn);
			player->DeletePlayerTargetPawn(GetPawn());
		
		}

		//BlackboardComp->SetValueAsObject(BBKEY_TARGET,nullptr);
		BlackboardComp->ClearValue(BBKEY_PROTECTFIELD);
		BlackboardComp->ClearValue(BBKEY_MYFIELD);
		BlackboardComp->ClearValue(BBKEY_TARGET);
		

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
	//SLOG(TEXT("StartTree"));
	RunAI();
	
}
