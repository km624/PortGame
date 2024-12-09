// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillNikke.h"
#include "Character/PGBaseCharacter.h"

USkillNikke::USkillNikke()
{
	SkillCooltime = 10.0f;
}

void USkillNikke::OnSkill()
{
	Super::OnSkill();
	IsCrouching = true;
	OnbIsCrouch(IsCrouching);
	FRotator NewRotation = ownercharacter->GetActorRotation() + FRotator(0, 90.0f, 0);
	ownercharacter->SetActorRotation(NewRotation);
}

void USkillNikke::OnbIsCrouch(bool crouching)
{
	APGBaseCharacter* skillcharacter = Cast<APGBaseCharacter>(ownercharacter);
	if (skillcharacter)
	{
		skillcharacter->SetbIsNikkeSkill(crouching);
		
	}
}

void USkillNikke::EndSkill()
{
	Super::EndSkill();
	IsCrouching = false;
	OnbIsCrouch(IsCrouching);

}
