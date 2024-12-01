// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGNpcCharacter.h"
#include "AI/PGAIController.h"
#include "PortGame/PortGame.h"
#include "Component/PGStatComponent.h"
#include "Physics/PGCollision.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Struct/PGGunStat.h"
#include "Data/GunWeaponData.h"
#include "Component/PGAttackComponent.h"


APGNpcCharacter::APGNpcCharacter()
{
	AIControllerClass = APGAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	PatrolRadius = 1000.0f;

	DetectRange = 1000.0f;

	TurnSpeed = 10.0f;



	Tags.Add(TAG_ENEMY);

	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
	
	bUseControllerRotationYaw = true;

}

void APGNpcCharacter::BeginPlay()
{
	Super::BeginPlay();

	//юс╫ц
	StatComponent->SetCurrentRarity(TEXT("NPC"));
	StatComponent->SetHitGauge(GetTotalStat().HitGauge);
	StatComponent->SetHp(GetTotalStat().MaxHp);

	if (CharacterType == EPlayerCharacterType::BlueArchive || CharacterType == EPlayerCharacterType::Nikke)
	{
		bIsAim = true;
		
	}
}

float APGNpcCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	if (DamageCauser->ActorHasTag(TAG_PLAYER))
	{
		if (bIsParry)
		{
			SLOG(TEXT("SuccesParray"));
			StatComponent->HitGaugeDamaged(GetTotalStat().HitGauge);
			
		}	
		else
			StatComponent->Damaged(DamageAmount);
	}

	return DamageAmount;
}

void APGNpcCharacter::NPCAttackHitStop(float time)
{
	CustomTimeDilation = 0.01f;
	GetWorld()->GetTimerManager().SetTimer(
		NPCHitStoptimerHandle,
		[this]() {
			CustomTimeDilation = 1.0f;
		}, time, false
	);
	
}

float APGNpcCharacter::GetPatrolRadius()
{

	return PatrolRadius;
}

float APGNpcCharacter::GetAIDetectRange()
{
	
	if (CharacterType == EPlayerCharacterType::BlueArchive || CharacterType == EPlayerCharacterType::Nikke)
	{
		UGunWeaponData* gundata = Cast<UGunWeaponData>(AttackComponent->GetWeaponData());
		if (gundata)
		{
			
			return DetectRange*1.25;
		}
	}
	
	return DetectRange;
}

float APGNpcCharacter::GetAIAttackRange(float targetDistance, APawn* pawn)
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
	

	return GetTotalStat().AttackRange*2;
}

void APGNpcCharacter::AttackByAI()
{
	
	AttackToComponent();
	
	bIsShoot = false;
	OnbIsShoot.Broadcast(bIsShoot);
}

void APGNpcCharacter::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void APGNpcCharacter::NotifyComboEnd()
{
	OnAttackFinished.ExecuteIfBound();
}

float APGNpcCharacter::AITurnSpeed()
{
	return TurnSpeed;
}

void APGNpcCharacter::OnParryStart()
{
	
	bIsParry = true;
	CustomTimeDilation = 0.25f;
	
		
}

void APGNpcCharacter::OnParryEnd()
{

	bIsParry = false;
	CustomTimeDilation = 1.0f;
}

bool APGNpcCharacter::GetBisParry() const
{
	return bIsParry;
}






