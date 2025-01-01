// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGAIBaseCharacter.h"
#include "Struct/PGGunStat.h"
#include "Data/GunWeaponData.h"
#include "Component/PGAttackComponent.h"
#include "AI/PGAIController.h"
#include "PortGame/PortGame.h"
#include "Components/CapsuleComponent.h"

APGAIBaseCharacter::APGAIBaseCharacter()
{

	//AIControllerClass = APGAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::Disabled;

	
}

void APGAIBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

float APGAIBaseCharacter::GetPatrolRadius()
{

	return PatrolRadius;
}

float APGAIBaseCharacter::GetAIDetectRange()
{

	if (CharacterType == EPlayerCharacterType::BlueArchive || CharacterType == EPlayerCharacterType::Nikke)
	{
		UGunWeaponData* gundata = Cast<UGunWeaponData>(AttackComponent->GetWeaponData());
		if (gundata)
		{

			return gundata->GunStat.traceDistance * 0.9f;
		}
	}

	return DetectRange;
}

float APGAIBaseCharacter::GetAIAttackRange(float targetDistance, APawn* pawn)
{
	if (pawn)
	{
		TargetPawn = pawn;

	}
	if (CharacterType == EPlayerCharacterType::BlueArchive || CharacterType == EPlayerCharacterType::Nikke)
	{
		
		UGunWeaponData* gundata = Cast<UGunWeaponData>(AttackComponent->GetWeaponData());
		if (gundata)
		{
			if (gundata->GunStat.traceDistance * 0.8f >= targetDistance && targetDistance > 200.0f)
			{
				/*bIsShoot = true;
				bIsAim = true;*/
				
				return gundata->GunStat.traceDistance * 0.8f;
			}
			//else if(gundata->GunStat.traceDistance * 0.8f < targetDistance)
			//{
			//	/*bIsShoot = false;
			//	bIsAim = false;*/
			//}
			
		}
	}

	/*bIsShoot = false;
	bIsAim = false;*/
	return 200.0f;
}

void APGAIBaseCharacter::AttackByAI()
{
	if (ShotCompeteTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ShotCompeteTimerHandle);
	}
	bIsAim = false;
	bIsShoot = false;
	AttackToComponent();
	
	
}

void APGAIBaseCharacter::ShotByAI()
{
	if (ShotCompeteTimerHandle.IsValid())
	{
		bIsAim = true;
		bIsShoot = true;
	}
	AttackToComponent();
	
	bIsShoot = false;
	OnbIsShoot.Broadcast(bIsShoot);
}

void APGAIBaseCharacter::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void APGAIBaseCharacter::NotifyComboEnd()
{
	OnAttackFinished.ExecuteIfBound();
}

float APGAIBaseCharacter::AITurnSpeed()
{
	return TurnSpeed;
}

bool APGAIBaseCharacter::CheckShotterType()
{
	if (CharacterType == EPlayerCharacterType::ETC) return false;

	return true;
}

void APGAIBaseCharacter::SetTimerShooterAttack(float shottime)
{
	GetWorld()->GetTimerManager().SetTimer(
		ShotCompeteTimerHandle,
		[this]() {

			bIsAim = false;
			bIsShoot = false;
			OnAttackFinished.ExecuteIfBound();
			GetWorld()->GetTimerManager().ClearTimer(ShotCompeteTimerHandle);

		}, shottime, false
	);
}

bool APGAIBaseCharacter::CheckTargetDead(APawn* Target)
{
	APGBaseCharacter* TargetCharacter = Cast<APGBaseCharacter>(Target);
	if (TargetCharacter)
	{
		if (TargetCharacter->GetbIsDead())
		{
			return true;
		}

	}
	return false;
}



void APGAIBaseCharacter::SetDead(int8 teamid)
{
	Super::SetDead(teamid);

	/*APGAIController* aiController = Cast<APGAIController>(GetController());
	if (aiController)
	{
		SLOG(TEXT("AI DEAD"));
		if (teamid!=0)
			aiController->TOMyFieldDead(teamid);
		aiController->AIIsDead();
	}*/
}

