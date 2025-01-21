// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGAIBaseCharacter.h"
#include "Struct/PGGunStat.h"
#include "Data/GunWeaponData.h"
#include "Component/PGAttackComponent.h"
#include "AI/PGAIController.h"
#include "PortGame/PortGame.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Character/PGPlayerCharacter.h"
#include "Component/PGStatComponent.h"


APGAIBaseCharacter::APGAIBaseCharacter()
{

	//AIControllerClass = APGAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::Disabled;

	
}


void APGAIBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CharacterOriginSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void APGAIBaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(ShotCompeteTimerHandle);
	
}

void APGAIBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APGAIController* aicontroller = Cast<APGAIController>(NewController);
	if (aicontroller)
	{
		MyAIController = aicontroller;
	}
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
			
				
				return gundata->GunStat.traceDistance * 0.8f;
			}
		
			
		}
	}

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

bool APGAIBaseCharacter::CheckTargetDead()
{
	
	if (bIsDead)
	{
		return true;
	}

	
	return false;
}

void APGAIBaseCharacter::NotVisibleAttack(APawn* Target)
{
	FDamageEvent DamageEvent;
	if (Target->GetController() != nullptr&& Target!=nullptr)
	{
		float AttackDamage = GetTotalStat().Attack;
		
		if (ActorHasTag(TAG_AI))
		{
			AttackDamage *= 0.5f;
		}

		Target->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	
	}
	
}

void APGAIBaseCharacter::ProtectOutRange(bool bisout)
{
	if (bisout)
	{
		
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
		SLOG(TEXT("characterspeed :  %f"), GetCharacterMovement()->MaxWalkSpeed);
	}
	else
	{
		SLOG(TEXT("In"));
		GetCharacterMovement()->MaxWalkSpeed =CharacterOriginSpeed;
	}
}



void APGAIBaseCharacter::BindDeadProtectTarget(APawn* protectTarget)
{
	//IAIBodyGuardInterface* player = Cast<IAIBodyGuardInterface>(protectTarget);
	APGPlayerCharacter* player = Cast<APGPlayerCharacter>(protectTarget);
	
	if (player)
	{
		OnDeadProtect.AddUObject(player, &APGPlayerCharacter::DeletePlayerProtectPawn);
	}
}



void APGAIBaseCharacter::SetDead(AActor* DamageCauser)
{
	Super::SetDead(DamageCauser);
	OnDeadProtect.Broadcast(this);
	OnDeadProtect.Clear();
	
}

