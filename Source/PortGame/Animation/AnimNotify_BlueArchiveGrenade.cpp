// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BlueArchiveGrenade.h"
#include "Skill/BlueGrenade.h"

void UAnimNotify_BlueArchiveGrenade::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

    if (MeshComp && MeshComp->GetOwner())
    {
        AActor* Owner = MeshComp->GetOwner();

        // ����ź Ŭ���� Ȯ��
        if (GrenadeClass)
        {
            FVector SpawnLocation = MeshComp->GetSocketLocation(FName("weaponRifleSocket")) + MeshComp->GetOwner()->GetActorForwardVector()*25.0f;
            
            FRotator SpawnRotation = MeshComp->GetOwner()->GetActorRotation() + FRotator(30.0f, 0.0f, 0.0f);
            ABlueGrenade* Grenade = Cast<ABlueGrenade>(GetWorld()->SpawnActorDeferred<ABlueGrenade>(
                GrenadeClass,
                FTransform(SpawnRotation, SpawnLocation),
                MeshComp->GetOwner(),
               nullptr
                ));

            if (Grenade)
            {
                Grenade->SetSkillOwnerCharacter(MeshComp->GetOwner());

                // ���͸� ���忡 ����
                Grenade->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
            }
        }
    }
}
