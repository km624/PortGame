// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillETC.h"
#include "Animation/AnimMontage.h"
#include "Character/PGPlayerCharacter.h"
#include "PortGame/PortGame.h"

const FString USkillETC::SkillMontage = TEXT("ETCSkillMontage");

USkillETC::USkillETC()
{
	/*static ConstructorHelpers::FObjectFinder<UAnimMontage>SkillMontage(TEXT("/Script/Engine.AnimMontage'/Game/PortGame/Animation/Skill/ETC/ETCSkillMontage.ETCSkillMontage'"));
	if (SkillMontage.Object)
	{
		SkillETCMontage = SkillMontage.Object;
	}*/

	SkillCooltime = 20.0f;
}

void USkillETC::SetSkillSetting(APGBaseCharacter* owner)
{
	Super::SetSkillSetting(owner);
	owner->LoadAndPlayMontageByPath(owner->CharacterName, SkillMontage);
}


void USkillETC::OnSkill()
{
	if (!bCanSkill) return;
	
	Super::OnSkill();
	
	
	PlaySkillETCMontage();
	
}

void USkillETC::PlaySkillETCMontage()
{
	
	UAnimInstance* AnimInstance = ownercharacter->GetMesh()->GetAnimInstance();
	
	float SkillSpeed = ownercharacter->GetTotalStat().AttackSpeed;
	AnimInstance->Montage_Play(ownercharacter->AllMontage[SkillMontage], SkillSpeed);
	//플레이어 캐릭터만 값 받아서 돌리기
	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(ownercharacter);
	if (playerCharacter)
	{
		playerCharacter->SetbIsAttackRotation(true);
	}

	EndDelegate.BindUObject(this, &USkillETC::EndPlaySkillETCMontage);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ownercharacter->AllMontage[SkillMontage]);
	
}

void USkillETC::EndPlaySkillETCMontage(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(ownercharacter);
	if (playerCharacter)
	{
		playerCharacter->SetbIsAttackRotation(false);
	}
	//SLOG(TEXT("END"));
	EndSkill();
}

void USkillETC::EndSkill()
{
	Super::EndSkill();
}


