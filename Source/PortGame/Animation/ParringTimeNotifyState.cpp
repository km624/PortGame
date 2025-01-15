// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ParringTimeNotifyState.h"
#include "Interface/NPCParryCheckInterface.h"
#include "PortGame/PortGame.h"
#include "Interface/AttackHitStopInterface.h"
void UParringTimeNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp)
	{
		INPCParryCheckInterface* NPC = Cast<INPCParryCheckInterface>(MeshComp->GetOwner());
		if(NPC)
			NPC->OnParryStart(TotalDuration);

		IAttackHitStopInterface* playerCharacter = Cast<IAttackHitStopInterface>(MeshComp->GetOwner());
		if (playerCharacter)
		{
			playerCharacter->AttackSlowStart();
		}
		//SLOG(TEXT("duration %f"), TotalDuration);
	}
}

void UParringTimeNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		INPCParryCheckInterface* NPC = Cast<INPCParryCheckInterface>(MeshComp->GetOwner());
		if (NPC)
		{
			NPC->OnParryEnd();
		}
		
		IAttackHitStopInterface* playerCharacter = Cast<IAttackHitStopInterface>(MeshComp->GetOwner());
		if (playerCharacter)
		{
			playerCharacter->AttackSlowEnd();
		}
	}

}
