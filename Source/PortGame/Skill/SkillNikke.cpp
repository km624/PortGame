// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillNikke.h"
#include "Character/PGBaseCharacter.h"
#include "Skill/NikkeWall.h"

USkillNikke::USkillNikke()
{
    static ConstructorHelpers::FClassFinder<ANikkeWall>SkillClass(TEXT("/Script/Engine.Blueprint'/Game/PortGame/Skill/Nikke/BP_NikkeWall.BP_NikkeWall_C'"));
    if (SkillClass.Class)
    {
        NikkeWallClass = SkillClass.Class;
    }
	SkillCooltime = 10.0f;
}

void USkillNikke::OnSkill()
{
    if (!bCanSkill) return;

	Super::OnSkill();
	
    // ����ź Ŭ���� Ȯ��
    if (NikkeWallClass)
    {
        FVector SpawnLocation = ownercharacter->GetActorLocation() + ownercharacter->GetActorForwardVector() * 150.0f + 
            ownercharacter->GetActorUpVector()*750.0f;

        FRotator SpawnRotation = ownercharacter->GetActorRotation();
        ANikkeWall* nikkewall = Cast<ANikkeWall>(GetWorld()->SpawnActorDeferred<ANikkeWall>(
            NikkeWallClass,
            FTransform(SpawnRotation, SpawnLocation),
            ownercharacter,
            nullptr
        ));

        if (nikkewall)
        {
            nikkewall->SetUpNikkeWall(ownercharacter, 10.0f);

            // ���͸� ���忡 ����
            nikkewall->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
        }
    }

    EndSkill();

}



void USkillNikke::EndSkill()
{
	Super::EndSkill();

}
