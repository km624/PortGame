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
			//ĳ���͵��� ���� �ϴ� üũ �Լ� ����
			AttackPawn->ComboSectionEnd();
		}
	}
}
