// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGNpcCharacter.h"
#include "AI/PGAIController.h"
#include "PortGame/PortGame.h"
#include "Component/PGStatComponent.h"
#include "Physics/PGCollision.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "Struct/PGGunStat.h"
//#include "Data/GunWeaponData.h"
//#include "Component/PGAttackComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"


APGNpcCharacter::APGNpcCharacter()
{
	/*AIControllerClass = APGAIController::StaticClass();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;*/

	/*PatrolRadius = 1000.0f;

	DetectRange = 1000.0f;

	TurnSpeed = 10.0f;*/

	Tags.Add(TAG_AI);

	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
	
	bUseControllerRotationYaw = true;

	ParryNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParryEffectComp"));
	ParryNiagaraComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> parryEffect(TEXT("/Script/Niagara.NiagaraSystem'/Game/PortGame/Effect/Niagara/NA_ParryState.NA_ParryState'"));
	if (parryEffect.Object)
	{
		NAParryEffect = parryEffect.Object;
	}

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
			
			StatComponent->HitGaugeDamaged(GetTotalStat().HitGauge);

			NAParryUpdateEnd();
			
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
	//SLOG(TEXT("NAme: %s , time: %f , origincustom: %f"),*GetActorLabel(), time,OrginTimeDilation);
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
	//OnAttackFinished.Unbind();
	GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);

	NAParryUpdateEnd();
	GetWorld()->GetTimerManager().ClearTimer(NAScaleTimerHandle);
	

	if (aiController)
	{
		SLOG(TEXT("AI DEAD"));
		aiController->BlackBoardReset();
		aiController->StopAI();
		aiController->StopMovement();
	}
}

//float APGNpcCharacter::GetPatrolRadius()
//{
//
//	return PatrolRadius;
//}
//
//float APGNpcCharacter::GetAIDetectRange()
//{
//	
//	if (CharacterType == EPlayerCharacterType::BlueArchive || CharacterType == EPlayerCharacterType::Nikke)
//	{
//		UGunWeaponData* gundata = Cast<UGunWeaponData>(AttackComponent->GetWeaponData());
//		if (gundata)
//		{
//			
//			return DetectRange*1.25;
//		}
//	}
//	
//	return DetectRange;
//}
//
//float APGNpcCharacter::GetAIAttackRange(float targetDistance, APawn* pawn)
//{
//	if (pawn)
//	{
//		TargetPawn = pawn;
//		
//	}
//	if (targetDistance > GetTotalStat().AttackRange * 2.5f)
//	{
//		if (CharacterType == EPlayerCharacterType::BlueArchive || CharacterType == EPlayerCharacterType::Nikke)
//		{
//			UGunWeaponData* gundata = Cast<UGunWeaponData>(AttackComponent->GetWeaponData());
//			if (gundata)
//			{
//				bIsShoot = true;
//				bIsAim = true;
//				
//				
//				return gundata->GunStat.traceDistance * 0.75;
//			}
//		}
//	}
//
//	bIsShoot = false;
//	bIsAim = false;
//	
//
//	return GetTotalStat().AttackRange*2;
//}
//
//void APGNpcCharacter::AttackByAI()
//{
//	
//	AttackToComponent();
//	
//	bIsShoot = false;
//	OnbIsShoot.Broadcast(bIsShoot);
//}
//
//void APGNpcCharacter::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
//{
//	OnAttackFinished = InOnAttackFinished;
//}
//
//void APGNpcCharacter::NotifyComboEnd()
//{
//	OnAttackFinished.ExecuteIfBound();
//}
//
//float APGNpcCharacter::AITurnSpeed()
//{
//	return TurnSpeed;
//}

void APGNpcCharacter::OnParryStart(float time)
{
	if (!TargetPawn->ActorHasTag(TAG_PLAYER)) return;

	if (bIsGroggy)return;

	if (RandomParry()==false) return;

	
	NAParryStart();
	bIsParry = true;
	CustomTimeDilation = 0.3f;
	float ActorTime = time / CustomTimeDilation;
	GetWorld()->GetTimerManager().SetTimer(NAScaleTimerHandle, [this, ActorTime]() { NAParryUpdateScale(ActorTime); }, 0.01f, true);
		
}

bool APGNpcCharacter::RandomParry()
{
	float RandomValue = FMath::FRand()*100.0f;
	if (ParryPercent > RandomValue)
		return true;
	return false;
}

void APGNpcCharacter::OnParryEnd()
{
	
	bIsParry = false;
	CustomTimeDilation = 1.0f;
	NAParryUpdateEnd();
}

bool APGNpcCharacter::GetBisParry() const
{
	return bIsParry;
}

void APGNpcCharacter::NAParryStart()
{
	
	if (NAParryEffect)
	{
		ParryNiagaraComponent->SetAsset(NAParryEffect);
		//NiagaraComponent->SetWorldLocation(GetActorForwardVector());
		if (TargetPawn)
		{
			FVector Direction = TargetPawn->GetActorLocation() - GetActorLocation();
			Direction.Z = 0; 
			FRotator NewRotation = Direction.Rotation();

			ParryNiagaraComponent->SetWorldRotation(NewRotation);
		}
		ParryNiagaraComponent->SetWorldScale3D(FVector(1.0f));
		ParryNiagaraComponent->Activate();
	}

}

void APGNpcCharacter::NAParryUpdateScale(float time)
{
	if(bIsGroggy)NAParryUpdateEnd();
	ElapsedTime += 0.01f; // Update 간격에 맞춰 누적 시간 증가

	// 스케일 계산
	float Alpha = FMath::Clamp(ElapsedTime / time, 0.0f, 1.0f);
	FVector NewScale = FMath::Lerp(FVector(1.0f), FVector(0.0f), Alpha);

	// 새로운 스케일 적용
	ParryNiagaraComponent->SetWorldScale3D(NewScale);

	// 종료 조건
	if (ElapsedTime >= time)
	{
		NAParryUpdateEnd();
	}
}

void APGNpcCharacter::NAParryUpdateEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(NAScaleTimerHandle);
	ElapsedTime = 0.0f; // 리셋
	
	ParryNiagaraComponent->Deactivate();
}








