// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_BlueArchiveGrenade.h"
#include "Skill/BlueGrenade.h"

void UAnimNotify_BlueArchiveGrenade::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    
    Super::Notify(MeshComp, Animation, EventReference);

    if (!MeshComp || !MeshComp->GetOwner())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid MeshComp or Owner in Notify"));
        return;
    }

    // 프리뷰 모드 확인
    if (MeshComp->GetWorld() && MeshComp->GetWorld()->WorldType == EWorldType::EditorPreview)
    {
        UE_LOG(LogTemp, Warning, TEXT("Skipping grenade spawn in Preview Mode"));
        return;
    }

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner || !GrenadeClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Owner or GrenadeClass in Notify"));
        return;
    }

    UWorld* World = MeshComp->GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World is invalid!"));
        return;
    }

    // Calculate spawn transform
    FVector SpawnLocation = MeshComp->GetSocketLocation(FName("weaponRifleSocket")) + Owner->GetActorForwardVector() * 25.0f;
    FRotator SpawnRotation = Owner->GetActorRotation() + FRotator(30.0f, 0.0f, 0.0f);

    // Spawn the grenade
    ABlueGrenade* Grenade = Cast<ABlueGrenade>(World->SpawnActorDeferred<ABlueGrenade>(
        GrenadeClass,
        FTransform(SpawnRotation, SpawnLocation),
        Owner,
        nullptr
    ));

    if (Grenade)
    {
        Grenade->SetSkillOwnerCharacter(Owner);
        Grenade->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn grenade!"));
    }
}
