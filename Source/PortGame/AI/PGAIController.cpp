// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PGAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "AI/PGAI.h"
#include "PortGame/PortGame.h"
#include "Interface/AITargetPlayerInterface.h"
#include "Field/PGField.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Engine/LevelScriptActor.h"
#include "Interface/LevelGameStartInterface.h"


APGAIController::APGAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
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

void APGAIController::SetForceMoveVector(FVector targetVector)
{
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (BlackboardComp)
	{
		if (targetVector == FVector::ZeroVector)
		{
			SLOG(TEXT("CantMovetoTarget"));
			return;
		}
		
		BlackboardComp->ClearValue(BBKEY_TARGET);
		BlackboardComp->SetValueAsVector(BBKEY_FORCEMOVEVECTOR, targetVector);
	}
}

void APGAIController::SetBodyGuardAttack(EAIAttackEnumData attackenum, FRotator attackrotator)
{
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(BBKEY_ATTACKENUM, static_cast<uint8>(attackenum));
		BlackboardComp->SetValueAsRotator(BBKEY_BGATTACKROT, attackrotator);
	}
}

void APGAIController::SetVisible(bool visible)
{
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(BBKEY_VISIBLE, visible);
	}
}

void APGAIController::RunAI()
{
	//SLOG(TEXT("RunAi"));
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		BlackboardComp->SetValueAsBool(BBKEY_VISIBLE, true);
		CheckGameStart();
		
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void APGAIController::BindGameStart()
{
	ILevelGameStartInterface* levelgamestart = Cast<ILevelGameStartInterface>(GetWorld()->GetLevelScriptActor());
	if (levelgamestart)
	{
		levelgamestart->SetGameStartAI(this);
	}
	
}

void APGAIController::CheckGameStart()
{
	ILevelGameStartInterface* levelgamestart = Cast<ILevelGameStartInterface>(GetWorld()->GetLevelScriptActor());
	if (levelgamestart)
	{
		SetGameStart(levelgamestart->GetbGameStart());
	}
}

void APGAIController::SetGameStart(bool bgameStart)
{
	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(BBKEY_GAMESTART, bgameStart);

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
				myfield->DamageField(GetPawn(),teamid);
		}
	}

}

void APGAIController::AIIsDead()
{
	StopAI();
	StopMovement();
	ClearFocus(EAIFocusPriority::Gameplay);
	BlackBoardReset();
	UnPossess();
	
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
		

		BlackboardComp->ClearValue(BBKEY_PROTECTFIELD);
		BlackboardComp->ClearValue(BBKEY_FORCEMOVEVECTOR);
		BlackboardComp->ClearValue(BBKEY_ATTACKENUM);
		BlackboardComp->ClearValue(BBKEY_MYFIELD);
		BlackboardComp->ClearValue(BBKEY_TARGET);
		BlackboardComp->ClearValue(BBKEY_PATROLPOS);
		BlackboardComp->ClearValue(BBKEY_CIRCLEPOS);
		BlackboardComp->ClearValue(BBKEY_OUTPROTECTRANGE);
		BlackboardComp->ClearValue(BBKEY_NEXTATTACKFIELD);
		BlackboardComp->ClearValue(BBKEY_PROTECTTARGET);
		BlackboardComp->ClearValue(BBKEY_PROTECTPOS);
		

	}
	
}


void APGAIController::BeginPlay()
{
	Super::BeginPlay();

	BindGameStart();
}

void APGAIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	RunAI();
}

void APGAIController::StopTree()
{
	
	StopMovement();
	StopAI();
	
	
}

void APGAIController::StartTree()
{
	//SLOG(TEXT("StartTree"));
	RunAI();
	
}
