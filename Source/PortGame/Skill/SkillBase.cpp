// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillBase.h"
#include "Character/PGBaseCharacter.h"
#include "TimerManager.h"
#include "PortGame/PortGame.h"

USkillBase::USkillBase()
{
	
}

void USkillBase::SetSkillSetting(APGBaseCharacter* owner)
{
	ownercharacter = owner;
	bCanSkill = true;
}

void USkillBase::OnSkill()
{
	if (!bCanSkill) return;
	
	SetTimer();
	bIsSkill = true;
	OnbIsSkill.Broadcast(true);
	
}

void USkillBase::SetTimer()
{
	bCanSkill = false;
	OnbCanSkill.Broadcast(bCanSkill);
	GetWorld()->GetTimerManager().SetTimer(
		CooldownTimerHandle,
		[this]() {
			SLOG(TEXT("CanSkill"));
			bCanSkill = true;
			OnbCanSkill.Broadcast(bCanSkill);
		}, SkillCooltime, false
	);

}

void USkillBase::EndSkill()
{
	
	bIsSkill = false;
	OnbIsSkill.Broadcast(false);
}
