// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ComboCheckAnimNotify.h"
#include "Interface/ComboCheckInterface.h"

void UComboCheckAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IComboCheckInterface* AttackPawn = Cast<IComboCheckInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			//캐릭터들의 공격 하는 체크 함수 실행
			AttackPawn->ComboSectionEnd();
		}
	}
}
