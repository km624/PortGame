// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGAIBaseCharacter.h"
#include "Struct/PGGunStat.h"
#include "Data/GunWeaponData.h"
#include "Component/PGAttackComponent.h"
#include "AI/PGAIController.h"

APGAIBaseCharacter::APGAIBaseCharacter()
{

	AIControllerClass = APGAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	
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

			return DetectRange * 1.25;
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
	if (targetDistance > GetTotalStat().AttackRange * 2.5f)
	{
		if (CharacterType == EPlayerCharacterType::BlueArchive || CharacterType == EPlayerCharacterType::Nikke)
		{
			UGunWeaponData* gundata = Cast<UGunWeaponData>(AttackComponent->GetWeaponData());
			if (gundata)
			{
				bIsShoot = true;
				bIsAim = true;


				return gundata->GunStat.traceDistance * 0.75;
			}
		}
	}

	bIsShoot = false;
	bIsAim = false;


	return GetTotalStat().AttackRange * 2;
}

void APGAIBaseCharacter::AttackByAI()
{

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

