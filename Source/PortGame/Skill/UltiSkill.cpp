// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/UltiSkill.h"
#include "Animation/AnimMontage.h"
#include "Character/PGPlayerCharacter.h"
#include "PortGame/PortGame.h"

UUltiSkill::UUltiSkill()
{
	SkillCooltime = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>SkillMontage(TEXT("/Script/Engine.AnimMontage'/Game/PortGame/Animation/Skill/UltiSkill/UltiSkillMontage.UltiSkillMontage'"));
	if (SkillMontage.Object)
	{
		UltiSkillMontage = SkillMontage.Object;
	}

}

void UUltiSkill::OnSkill()
{
	Super::OnSkill();


	PlayUltiSkillMontage();
}

void UUltiSkill::PlayUltiSkillMontage()
{
	UAnimInstance* AnimInstance = ownercharacter->GetMesh()->GetAnimInstance();

	
	AnimInstance->Montage_Play(UltiSkillMontage, 1.0f);

	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(ownercharacter);
	if (playerCharacter)
	{
		playerCharacter->SetbIsAttackRotation(true);
	}

	EndDelegate.BindUObject(this, &UUltiSkill::EndUltiSkilleMontage);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, UltiSkillMontage);
}

void UUltiSkill::EndUltiSkilleMontage(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(ownercharacter);
	if (playerCharacter)
	{
		playerCharacter->SetbIsAttackRotation(false);
	}
	EndSkill();
}

void UUltiSkill::EndSkill()
{
	Super::EndSkill();
}
