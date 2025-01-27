// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ProtectPlayer/BTService_CheckPlayerRange.h"
#include "AI/PGAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/PGAICharacterInterface.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
//#include "Character/PGPlayerCharacter.h"

UBTService_CheckPlayerRange::UBTService_CheckPlayerRange()
{
	MaxDistance = 500.0f;
	Interval = 1.0f;
	NodeName = TEXT("CheckOutRange");
}

void UBTService_CheckPlayerRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == NULL)
	{
		return;
	}

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

	/*APawn* ProtectPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PROTECTTARGET));
	if (nullptr == ProtectPawn)
	{
		return;
	}*/
	AActor* ProtectPos = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PROTECTPOSACTOR));
	if (nullptr == ProtectPos)
	{
		return;
	}

	// 플레이어의 위치 가져오기
	FVector TargetLocation = ProtectPos->GetActorLocation();
	FVector ControllingLocation = ControllingPawn->GetActorLocation();
	float TargetDistance = FVector::Distance(TargetLocation, ControllingLocation);

	bool result = false;

	if (TargetDistance > MaxDistance)
	{
		result = true;
	}
	else
	{
		result = false;
	}

	bool currentresult = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_OUTPROTECTRANGE);
	if (currentresult != result)
	{
		AIPawn->ProtectOutRange(result);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_OUTPROTECTRANGE, result);
		
		
	}
		


}
