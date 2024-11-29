// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ComboCheckAnimNotifyState.h"
#include "Interface/PlayerAttackInterface.h"
#include "PortGame/PortGame.h"

void UComboCheckAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp)
	{


		IPlayerAttackInterface* AttackPawn = Cast<IPlayerAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->ComboCheckStartToComp();
		}
	}
}

void UComboCheckAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp)
	{

		IPlayerAttackInterface* AttackPawn = Cast<IPlayerAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->ComboCheckEndToComp();
		}
	}
}
