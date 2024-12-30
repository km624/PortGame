// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TargetCirclePos.h"
#include "PGAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "Interface/PGNPCCharacterInterface.h"
#include "Interface/PGAICharacterInterface.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
#include "Character/PGBaseCharacter.h"


UBTTask_TargetCirclePos::UBTTask_TargetCirclePos()
{

}

EBTNodeResult::Type UBTTask_TargetCirclePos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == NULL)
	{
		return EBTNodeResult::Failed;
	}

	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return EBTNodeResult::Failed;
	}

	//�������� �Ǽ� V1
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == NULL)
	{
		return EBTNodeResult::Failed;
	}

	IPGAICharacterInterface* AIPawn = Cast<IPGAICharacterInterface>(ControllingPawn);
	if (AIPawn == NULL)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	
	// �÷��̾��� ��ġ ��������
	FVector TargetLocation = TargetPawn->GetActorLocation();
	FVector ControllingLocation = ControllingPawn->GetActorLocation();
	float TraceDistance = AIPawn->GetAIAttackRange(ControllingPawn->GetDistanceTo(TargetPawn), TargetPawn)*1.5f;

	APGBaseCharacter* character = Cast<APGBaseCharacter>(ControllingPawn);
	if (character)
	{
		character->SetbIsAim(false);
		character->SetbIsShoot(false);
	}
	if (TraceDistance < 350.0f)
	{
		TraceDistance = 350.0f;
	}
	// ���� ���� ���� ���
	FVector DirectionToPlayer = (TargetLocation - ControllingLocation).GetSafeNormal();

	// ���� ���Ϳ��� ���� ���
	float PlayerAngle = FMath::Atan2(DirectionToPlayer.Y, DirectionToPlayer.X) * 180.0f / PI; // ���ȿ��� ���� ��ȯ

	// 60�� ���� ������ ���� ���� ���� (��30��)
	float  RandomAngle = FMath::RandRange(PlayerAngle - 60.0f, PlayerAngle + 60.0f);
	float Radian = FMath::DegreesToRadians(RandomAngle+180.0f); // ���� �������� ��ȯ

	
	FVector NewLocation = TargetLocation + FVector(FMath::Cos(Radian) * TraceDistance, FMath::Sin(Radian) * TraceDistance, 0.0f);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_CIRCLEPOS, NewLocation);

	DrawDebugPoint(World, NewLocation, 10.0f, FColor::Blue, false, 1.0f);
	DrawDebugLine(World, ControllingPawn->GetActorLocation(), NewLocation, FColor::Blue, false, 1.0f);

	

	return EBTNodeResult::Succeeded;


}
