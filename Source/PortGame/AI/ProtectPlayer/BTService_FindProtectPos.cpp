 // Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ProtectPlayer/BTService_FindProtectPos.h"
#include "AI/PGAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/PGAICharacterInterface.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
//#include "Character/PGPlayerCharacter.h"
#include "Interface/AIBodyGuardInterface.h"

UBTService_FindProtectPos::UBTService_FindProtectPos()
{
	Interval = 0.2f;
	/*OffsetX = 150.0f;
	OffsetY = 250.0f;*/

}

void UBTService_FindProtectPos::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	APawn* ProtectPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PROTECTTARGET));
	if (nullptr == ProtectPawn)
	{
		return;
	}


	// 플레이어의 위치 가져오기
	FVector TargetLocation = ProtectPawn->GetActorLocation();
	FVector TargetForwardLocation = ProtectPawn->GetActorForwardVector();
	FVector TArgetRightLoacation = ProtectPawn->GetActorRightVector();
	FVector ControllingLocation = ControllingPawn->GetActorLocation();
	
	
	//APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(ProtectPawn);
	IAIBodyGuardInterface* playerCharacter = Cast<IAIBodyGuardInterface>(ProtectPawn);
	if (nullptr == playerCharacter)
	{
		return;

	}
	FVector Offset = playerCharacter->CalculateOffsetPawn(ControllingPawn);
	if (Offset==FVector::Zero())
	{
		return;
	}

	FVector AILocation = TargetLocation + Offset;
	

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PROTECTPOS, AILocation);


	DrawDebugPoint(World, AILocation, 10.0f, FColor::Blue, false, 1.0f);
	DrawDebugLine(World, ControllingPawn->GetActorLocation(), AILocation, FColor::Blue, false, 1.0f);




}
