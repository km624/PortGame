// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"

#include "Interface/PlayerAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		
		
		IPlayerAttackInterface* AttackPawn = Cast<IPlayerAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			//캐릭터들의 공격 하는 체크 함수 실행
			AttackPawn->AttackHitCheck();
		}
	}
}
