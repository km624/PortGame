// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillNikke.h"
#include "Character/PGBaseCharacter.h"
#include "Skill/NikkeWall.h"
#include "Character/PGPlayerCharacter.h"

const FString USkillNikke::SkillMontage = TEXT("NikkeSkillMontage");

USkillNikke::USkillNikke()
{
    static ConstructorHelpers::FClassFinder<ANikkeWall>SkillClass(TEXT("/Script/Engine.Blueprint'/Game/PortGame/Skill/Nikke/BP_NikkeWall.BP_NikkeWall_C'"));
    if (SkillClass.Class)
    {
        NikkeWallClass = SkillClass.Class;
    }
   /* static ConstructorHelpers::FObjectFinder<UAnimMontage>SkillMontage(TEXT("/Script/Engine.AnimMontage'/Game/PortGame/Animation/Skill/Nikke/NikkeSkillPhoneMontage.NikkeSkillPhoneMontage'"));
    if (SkillMontage.Object)
    {
        SkillNikkeMontage = SkillMontage.Object;
    }*/

	SkillCooltime = 30.0f;
}

void USkillNikke::SetSkillSetting(APGBaseCharacter* owner)
{
    Super::SetSkillSetting(owner);
    owner->LoadAndPlayMontageByPath(owner->CharacterName, SkillMontage);
}

void USkillNikke::OnSkill()
{
    if (!bCanSkill) return;

	Super::OnSkill();
	

    GetWorld()->GetTimerManager().SetTimer(SpawnWallTimerHandle,this,&ThisClass::SpawnNikkeWaill,SpawnDelayTime , false);
    PlaySkillNikkeMontage();

}

void USkillNikke::PlaySkillNikkeMontage()
{
    UAnimInstance* AnimInstance = ownercharacter->GetMesh()->GetAnimInstance();

   
    AnimInstance->Montage_Play(ownercharacter->AllMontage[SkillMontage], 1.0f);

    APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(ownercharacter);
    if (playerCharacter)
    {
        playerCharacter->SetbIsAttackRotation(true);
    }

    EndDelegate.BindUObject(this, &USkillNikke::EndPSkillNikkeMontage);
    AnimInstance->Montage_SetEndDelegate(EndDelegate, ownercharacter->AllMontage[SkillMontage]);
}

void USkillNikke::SpawnNikkeWaill()
{
    if (NikkeWallClass)
    {
        FVector SpawnLocation = ownercharacter->GetActorLocation() + ownercharacter->GetActorForwardVector() * 150.0f +
            ownercharacter->GetActorUpVector() * 750.0f;

        FRotator SpawnRotation = ownercharacter->GetActorRotation();
        ANikkeWall* nikkewall = Cast<ANikkeWall>(GetWorld()->SpawnActorDeferred<ANikkeWall>(
            NikkeWallClass,
            FTransform(SpawnRotation, SpawnLocation),
            ownercharacter,
            nullptr
        ));

        if (nikkewall)
        {
            nikkewall->SetUpNikkeWall(ownercharacter, SkillWallTime);

            // 액터를 월드에 스폰
            nikkewall->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
        }
    }

}

void USkillNikke::EndPSkillNikkeMontage(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
    GetWorld()->GetTimerManager().ClearTimer(SpawnWallTimerHandle);

    APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(ownercharacter);
    if (playerCharacter)
    {
        playerCharacter->SetbIsAttackRotation(false);
    }

    EndSkill();
}



void USkillNikke::EndSkill()
{
	Super::EndSkill();

}
