// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGNpcCharacter.h"
#include "AI/PGAIController.h"
#include "PortGame/PortGame.h"
#include "Component/PGStatComponent.h"
#include "Physics/PGCollision.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Field/ObjectPoolManager.h"
#include "Engine/LevelScriptActor.h"
#include "Interface/ObjectPoolingInterface.h"
#include "Component/PGAttackComponent.h"


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
	ParryNiagaraComponent->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> parryEffect(TEXT("/Script/Niagara.NiagaraSystem'/Game/PortGame/Effect/Niagara/NA_ParryState.NA_ParryState'"));
	if (parryEffect.Object)
	{
		NAParryEffect = parryEffect.Object;
	}

}

void APGNpcCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (CharacterType == EPlayerCharacterType::BlueArchive || CharacterType == EPlayerCharacterType::Nikke)
	{
		bIsAim = true;
		
	}
	//NPC 캐릭터 팀 색깔 설정
	ChangeNpcColor();
}

void APGNpcCharacter::ChangeNpcColor()
{
	
	// 팀 색상 설정
	FLinearColor TeamColor = (TeamId != 1) ? FLinearColor::Red : FLinearColor::Blue;

	// 캐릭터의 메쉬에서 현재 머티리얼 가져오기
	UMaterialInterface* CurrentMaterial = GetMesh()->GetMaterial(0);
	if (CurrentMaterial)
	{
		// 동적 머티리얼이 이미 설정되어 있다면 재사용
		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(CurrentMaterial, this);
			GetMesh()->SetMaterial(0, DynamicMaterial); // 한 번만 설정
		}

		// 동적 머티리얼의 파라미터 업데이트
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue(TEXT("Tint"), TeamColor);
		}
	}
}



float APGNpcCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (!EventInstigator->GetPawn()) return DamageAmount;
	APGBaseCharacter* attackPawn = Cast<APGBaseCharacter>(EventInstigator->GetPawn());
	
	if (attackPawn)
	{
		//적팀일시
		if (GetTeamAttitudeTowards(*DamageCauser) && !DamageCauser->ActorHasTag(TAG_GRENADE))
		{
			//패리중일때
			if (bIsParry)
			{

				StatComponent->HitGaugeDamaged(GetTotalStat().HitGauge);

				NAParryUpdateEnd();

			}
			else
				StatComponent->Damaged(DamageAmount, attackPawn->GetGenericTeamId());
		}

		//수류탄에 맞았을시
		if (DamageCauser->ActorHasTag(TAG_GRENADE))
		{

			FVector Direction = GetActorLocation() - DamageCauser->GetActorLocation();
			Direction.Normalize();
			HitImpulseVector = Direction * 750.0f + (FVector(0, 0, 1) * 100.0f);

			if (GetTeamAttitudeTowards(*EventInstigator->GetPawn()))
			{
				StatComponent->Damaged(DamageAmount, attackPawn->GetGenericTeamId());
			}
			else
			{
				StatComponent->Damaged(DamageAmount * 0.3f, attackPawn->GetGenericTeamId());
			}
		}
	}
	
	return DamageAmount;
}

void APGNpcCharacter::NPCAttackHitStop(float time)
{
	if (bIsDead)return;
	if (currentSlowtime >= time)return;
	GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);
	currentSlowtime = time;
	
	CustomTimeDilation = 0.01f;
	GetWorld()->GetTimerManager().SetTimer(
		NPCHitStoptimerHandle,
		[this]() {
			CustomTimeDilation = 1.0f;
			GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);
		}, time, false
	);
	
}

void APGNpcCharacter::SetDead(int8 teamid)
{
	Super::SetDead(teamid);

	CustomTimeDilation = 1.0f;

	GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);

	NAParryUpdateEnd();

	GetWorld()->GetTimerManager().ClearTimer(NAScaleTimerHandle);
	
	GetWorld()->GetTimerManager().SetTimer(
		DeadHiddentimerHandle,
		[this]() {
			
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				AnimInstance->StopAllMontages(0.0f);
			}
			AttackComponent->ReturnWeaponPool();
			ReturnCharacterToPool();
			GetWorld()->GetTimerManager().ClearTimer(DeadHiddentimerHandle);
		}, 2.0f, false);

}

void APGNpcCharacter::ReturnCharacterToPool()
{
	IObjectPoolingInterface* poolmanager = Cast<IObjectPoolingInterface>(GetWorld()->GetLevelScriptActor());
	if (poolmanager)
	{
		poolmanager->GetObjectPoolManager()->ReturnObjectToPool(this);
	}
}


void APGNpcCharacter::PlayHitMontage()
{
	Super::PlayHitMontage();

	NAParryUpdateEnd();
	GetWorld()->GetTimerManager().ClearTimer(NAScaleTimerHandle);

}

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








