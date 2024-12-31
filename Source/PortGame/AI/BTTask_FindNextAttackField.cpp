// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindNextAttackField.h"
#include "PGAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/PGAICharacterInterface.h"
#include  "Interface/FieldManagerInterface.h"
#include "Field/FieldManager.h"
#include "GameLevel/PGGameLevelScriptActor.h"
#include "Engine/LevelScriptActor.h"
#include "PortGame/PortGame.h"
#include "Character/PGBaseCharacter.h"

UBTTask_FindNextAttackField::UBTTask_FindNextAttackField()
{
}

EBTNodeResult::Type UBTTask_FindNextAttackField::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	IFieldManagerInterface* levelactor =  Cast<IFieldManagerInterface>(ControllingPawn->GetWorld()->GetLevelScriptActor());
	if (levelactor == NULL)
	{
		
		return EBTNodeResult::Failed;
	}

	UFieldManager* fieldmanager = levelactor->GetFieldManager();
	if (fieldmanager == NULL)
	{
		
		return EBTNodeResult::Failed;
	}

	FVector AttackfieldLoc = fieldmanager->FoundEnemyField(ControllingPawn);
	
	if (AttackfieldLoc==FVector::ZeroVector)
	{
		
		return EBTNodeResult::Failed;
	}

	APGBaseCharacter* basecharacter = Cast<APGBaseCharacter>(ControllingPawn);
	if (basecharacter)
	{
		basecharacter->SetbIsAim(false);
		basecharacter->SetbIsShoot(false);
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_NEXTATTACKFIELD, AttackfieldLoc);
	
	return EBTNodeResult::Succeeded;
}
