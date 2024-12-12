// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/UltiSkill.h"
#include "Animation/AnimMontage.h"
#include "Character/PGPlayerCharacter.h"
#include "PortGame/PortGame.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PGPlayerCharacter.h"
#include "LevelSequenceActor.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"

UUltiSkill::UUltiSkill()
{
	SkillCooltime = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>SkillMontage(TEXT("/Script/Engine.AnimMontage'/Game/PortGame/Animation/Skill/UltiSkill/UltiSkillMontage.UltiSkillMontage'"));
	if (SkillMontage.Object)
	{
		UltiSkillMontage = SkillMontage.Object;
	}

	LevelSequenceActorClass = ALevelSequenceActor::StaticClass();


}

void UUltiSkill::UltiSkillSequenceSet(ULevelSequence* newsequence)
{
	LevelSequence = newsequence;
}



void UUltiSkill::OnSkill()
{
	bIsSkill = true;
	OnbIsSkill.Broadcast(true);

	GetWorld()->GetTimerManager().SetTimer(
		FirstUltiSkillTimerHandle,
		[this]() {
			StartCinematic();
		}, UltiDealy, false
	);


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

void UUltiSkill::StartCinematic()
{
	bIsCutscene = true;
	OnBIsCutscened.Broadcast(bIsCutscene);
	// 레벨 시퀀서 액터 생성
	LevelSequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>(LevelSequenceActorClass,ownercharacter->GetActorLocation(),
		ownercharacter->GetActorRotation());

	if (LevelSequenceActor)
	{
		// 레벨 시퀀스 설정
		if (LevelSequence)
		{
			LevelSequenceActor->SetSequence(LevelSequence);

			LevelSequencePlayer = LevelSequenceActor->SequencePlayer;
			// 컷씬 재생
			if (LevelSequencePlayer)
			{
				FMovieSceneObjectBindingID id = LevelSequence->FindBindingByTag(TEXT("Character"));
				if (id.IsValid())
				{
					
					LevelSequenceActor->SetBinding(id, { ownercharacter });

				}

				LevelSequencePlayer->Play();

				LevelSequencePlayer->OnFinished.AddDynamic(this, &UUltiSkill::OnLevelSequenceFinished);
			}
		}
	}
}

void UUltiSkill::OnLevelSequenceFinished()
{
	bIsCutscene = false;
	OnBIsCutscened.Broadcast(bIsCutscene);
	// LevelSequenceActor 삭제
	if (LevelSequenceActor)
	{
		LevelSequenceActor->Destroy();
	}
	PlayUltiSkillMontage();
}


