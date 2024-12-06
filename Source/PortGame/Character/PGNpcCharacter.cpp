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



	Tags.Add(TAG_AI);

	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
	
	bUseControllerRotationYaw = true;

}

void APGNpcCharacter::BeginPlay()
{
	Super::BeginPlay();

	//임시
	StatComponent->SetCurrentRarity(TEXT("NPC"));
	StatComponent->SetHitGauge(GetTotalStat().HitGauge);
	StatComponent->SetHp(GetTotalStat().MaxHp);

	if (CharacterType == EPlayerCharacterType::BlueArchive || CharacterType == EPlayerCharacterType::Nikke)
	{
		bIsAim = true;
		
	}

	//NPC 캐릭터 팀 색깔 설정
	FLinearColor teamcolor = (TeamId != 1) ? FLinearColor::Red : FLinearColor::Blue;
	
	UMaterialInterface* CurrentMaterial = GetMesh()->GetMaterial(0);
	if (CurrentMaterial)
	{
		
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CurrentMaterial, this);

		if (DynamicMaterial)
		{
			
			DynamicMaterial->SetVectorParameterValue(TEXT("Tint"), teamcolor);

			
			GetMesh()->SetMaterial(0, DynamicMaterial);
			if (GetMesh()->GetMaterial(0) == DynamicMaterial)
			{
				UE_LOG(LogTemp, Log, TEXT("Material Applied Successfully"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Material Not Applied"));
			}
		}
		
	}
}

float APGNpcCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	//if (DamageCauser->ActorHasTag(TAG_PLAYER))
	if (GetTeamAttitudeTowards(*DamageCauser))
	{
		if (bIsParry)
		{
			//SLOG(TEXT("SuccesParray"));
			StatComponent->HitGaugeDamaged(GetTotalStat().HitGauge);
			
		}	
		else
			StatComponent->Damaged(DamageAmount);
	}

	return DamageAmount;
}

void APGNpcCharacter::NPCAttackHitStop(float time)
{
	if (bIsDead)return;
	if (currentSlowtime > time)return;
	GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);
	currentSlowtime = time;
	float OrginTimeDilation = CustomTimeDilation;
	SLOG(TEXT("NAme: %s , time: %f , origincustom: %f"),*GetActorLabel(), time,OrginTimeDilation);
	CustomTimeDilation = 0.01f;
	GetWorld()->GetTimerManager().SetTimer(
		NPCHitStoptimerHandle,
		[this, OrginTimeDilation]() {
			CustomTimeDilation = OrginTimeDilation;
			GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);
		}, time, false
	);
	
}

void APGNpcCharacter::SetDead()
{
	Super::SetDead();

	APGAIController* aiController = Cast<APGAIController>(GetController());
	
	//죽으면 모두 초기화
	OnAttackFinished.Unbind();
	GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);
	if (aiController)
	{
		SLOG(TEXT("AI DEAD"));
		aiController->BlackBoardReset();
		aiController->StopAI();
		aiController->StopMovement();
	}
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
	StartNiagaraEffect();
	
	bIsParry = true;
	CustomTimeDilation = 0.3f;
	
		
}

void APGNpcCharacter::OnParryEnd()
{
	OnNiagaraSystemFinished(NiagaraComponent);
	bIsParry = false;
	CustomTimeDilation = 1.0f;
}

bool APGNpcCharacter::GetBisParry() const
{
	return bIsParry;
}






