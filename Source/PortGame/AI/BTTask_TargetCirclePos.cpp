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

	//버전업이 되서 V1
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

	// 0에서 360도 사이의 랜덤 각도 생성 (라디안으로 변환)
	//float RandomAngle = FMath::RandRange(0.0f, 360.0f); // 0도에서 360도까지 랜덤
	//float Radian = FMath::DegreesToRadians(RandomAngle); // 도를 라디안으로 변환

	//// 플레이어의 위치 가져오기
	//FVector TargetLocation = TargetPawn->GetActorLocation();
	//FVector ControllingLocation = ControllingPawn->GetActorLocation();

	//// 상대방 방향 벡터 계산
	//FVector DirectionToPlayer = (TargetLocation - ControllingLocation).GetSafeNormal();

	//// 방향 벡터에서 각도 계산
	//float PlayerAngle = FMath::Atan2(DirectionToPlayer.Y, DirectionToPlayer.X) * 180.0f / PI; // 라디안에서 도로 변환

	//// 90도 범위 내에서 랜덤 각도 생성 (±45도)
	//float RandomAngle = FMath::RandRange(PlayerAngle -30.0f, PlayerAngle + 30.0f);
	//float Radian = FMath::DegreesToRadians(RandomAngle); // 도를 라디안으로 변환

	//FVector NewLocation = TargetLocation + FVector(FMath::Cos(Radian) * 300.0f, FMath::Sin(Radian) * 300.0f, 0.0f);


	// 플레이어의 위치 가져오기
	FVector TargetLocation = TargetPawn->GetActorLocation();
	FVector ControllingLocation = ControllingPawn->GetActorLocation();
	float TraceDistance = AIPawn->GetAIAttackRange(ControllingPawn->GetDistanceTo(TargetPawn), TargetPawn)/0.5f;

	// 상대방 방향 벡터 계산
	FVector DirectionToPlayer = (TargetLocation - ControllingLocation).GetSafeNormal();

	// 방향 벡터에서 각도 계산
	float PlayerAngle = FMath::Atan2(DirectionToPlayer.Y, DirectionToPlayer.X) * 180.0f / PI; // 라디안에서 도로 변환

	// 60도 범위 내에서 랜덤 각도 생성 (±30도)
	float  RandomAngle = FMath::RandRange(PlayerAngle - 60.0f, PlayerAngle + 60.0f);
	float Radian = FMath::DegreesToRadians(RandomAngle+180.0f); // 도를 라디안으로 변환

	// TargetPawn의 위치를 기준으로 새로운 위치 계산 (반지름 300미터)
	FVector NewLocation = TargetLocation + FVector(FMath::Cos(Radian) * TraceDistance, FMath::Sin(Radian) * TraceDistance, 0.0f);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_CIRCLEPOS, NewLocation);

	DrawDebugPoint(World, NewLocation, 10.0f, FColor::Blue, false, 1.0f);
	DrawDebugLine(World, ControllingPawn->GetActorLocation(), NewLocation, FColor::Blue, false, 1.0f);

	return EBTNodeResult::Succeeded;


	return EBTNodeResult::Failed;
}
