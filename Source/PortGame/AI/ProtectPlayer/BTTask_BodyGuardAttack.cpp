// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ProtectPlayer/BTTask_BodyGuardAttack.h"
#include "AI/PGAI.h"
#include "AIController.h"
//#include "Interface/PGNPCCharacterInterface.h"
#include "Interface/PGAICharacterInterface.h"
#include "PortGame/PortGame.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_BodyGuardAttack::UBTTask_BodyGuardAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BodyGuardAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);


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

	FAICharacterAttackFinished OnAttackFinished;
	//델리게이트를 바인드 람다로
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AttackEnumData = StaticCast<EAIAttackEnumData>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BBKEY_ATTACKENUM));

	

	AIPawn->SetAIAttackDelegate(OnAttackFinished);

	
	if (AttackEnumData == EAIAttackEnumData::NormalAttack)
	{
		AIPawn->AttackByAI();
	}
	else if (AttackEnumData == EAIAttackEnumData::SkillAttack)
	{
		AIPawn->SkillByAI();
	}
	else
	{
		AIPawn->UltiSkillByAI();
	}
	//AIPawn->AttackByAI();
	
	return EBTNodeResult::InProgress;

}

void UBTTask_BodyGuardAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (AttackEnumData == EAIAttackEnumData::NormalAttack)
	{
		APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
		if (nullptr == ControllingPawn)
		{
			return;
		}


		IPGAICharacterInterface* AIPawn = Cast<IPGAICharacterInterface>(ControllingPawn);
		if (nullptr == AIPawn)
		{
			return;
		}
		AIPawn->AttackByAI();
	}
	
}

void UBTTask_BodyGuardAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (TaskResult == EBTNodeResult::Succeeded)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->ClearValue(BBKEY_ATTACKENUM);
		}
	}
}
