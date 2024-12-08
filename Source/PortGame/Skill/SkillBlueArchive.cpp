// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillBlueArchive.h"
#include "Animation/AnimMontage.h"
#include "Character/PGPlayerCharacter.h"
#include "PortGame/PortGame.h"

USkillBlueArchive::USkillBlueArchive()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage>SkillMontage(TEXT("/Script/Engine.AnimMontage'/Game/PortGame/Animation/Skill/BlueArchive/TosGrenadeUEMontage.TosGrenadeUEMontage'"));
	if (SkillMontage.Object)
	{
		SkillBlueArchiveMontage = SkillMontage.Object;
	}
	

	SkillCooltime = 10.0f;


}

void USkillBlueArchive::OnSkill()
{
	if (!bCanSkill) return;

	Super::OnSkill();

	
	PlaySkillgrenadeMontage();
}

void USkillBlueArchive::PlaySkillgrenadeMontage()
{
	UAnimInstance* AnimInstance = ownercharacter->GetMesh()->GetAnimInstance();

	float SkillSpeed = ownercharacter->GetTotalStat().AttackSpeed;
	AnimInstance->Montage_Play(SkillBlueArchiveMontage, SkillSpeed);
	
	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(ownercharacter);
	if (playerCharacter)
	{
		playerCharacter->SetbIsAttackRotation(true);
	}

	EndDelegate.BindUObject(this, &USkillBlueArchive::EndPSkillgrenadeMontage);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, SkillBlueArchiveMontage);
}

void USkillBlueArchive::EndPSkillgrenadeMontage(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(ownercharacter);
	if (playerCharacter)
	{
		playerCharacter->SetbIsAttackRotation(false);
	}
	EndSkill();
}

void USkillBlueArchive::EndSkill()
{
	Super::EndSkill();
}
