// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ParringTimeNotifyState.h"
#include "Interface/NPCParryCheckInterface.h"
#include "PortGame/PortGame.h"

void UParringTimeNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp)
	{
		INPCParryCheckInterface* NPC = Cast<INPCParryCheckInterface>(MeshComp->GetOwner());
		if(NPC)
			NPC->OnParryStart(TotalDuration);
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
		
	}

}
