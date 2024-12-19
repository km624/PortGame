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

	//�ӽ�
	/*StatComponent->SetCurrentRarity(TEXT("NPC"));
	StatComponent->SetHitGauge(GetTotalStat().HitGauge);
	StatComponent->SetHp(GetTotalStat().MaxHp);*/

	if (CharacterType == EPlayerCharacterType::BlueArchive || CharacterType == EPlayerCharacterType::Nikke)
	{
		bIsAim = true;
		
	}
	//NPC ĳ���� �� ���� ����
	ChangeNpcColor();
}

void APGNpcCharacter::ChangeNpcColor()
{
	
	// �� ���� ����
	FLinearColor TeamColor = (TeamId != 1) ? FLinearColor::Red : FLinearColor::Blue;

	// ĳ������ �޽����� ���� ��Ƽ���� ��������
	UMaterialInterface* CurrentMaterial = GetMesh()->GetMaterial(0);
	if (CurrentMaterial)
	{
		// ���� ��Ƽ������ �̹� �����Ǿ� �ִٸ� ����
		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(CurrentMaterial, this);
			GetMesh()->SetMaterial(0, DynamicMaterial); // �� ���� ����
		}

		// ���� ��Ƽ������ �Ķ���� ������Ʈ
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue(TEXT("Tint"), TeamColor);
		}
	}
}



float APGNpcCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	APGBaseCharacter* attackPawn = Cast<APGBaseCharacter>(EventInstigator->GetPawn());
	
	
	//�����Ͻ�
	if (GetTeamAttitudeTowards(*DamageCauser) && !DamageCauser->ActorHasTag(TAG_GRENADE))
	{
		//�и����϶�
		if (bIsParry)
		{
			
			StatComponent->HitGaugeDamaged(GetTotalStat().HitGauge);

			NAParryUpdateEnd();
			
		}	
		else
			StatComponent->Damaged(DamageAmount, attackPawn->GetGenericTeamId());
	}

	//����ź�� �¾�����
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
			StatComponent->Damaged(DamageAmount*0.3f, attackPawn->GetGenericTeamId());
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

	APGAIController* aiController = Cast<APGAIController>(GetController());
	
	//������ ��� �ʱ�ȭ
	//OnAttackFinished.Unbind();
	GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);

	NAParryUpdateEnd();
	GetWorld()->GetTimerManager().ClearTimer(NAScaleTimerHandle);
	

	/*if (aiController)
	{
		SLOG(TEXT("AI DEAD"));
		aiController->BlackBoardReset();
		aiController->StopAI();
		aiController->StopMovement();
	}*/
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
	ElapsedTime += 0.01f; // Update ���ݿ� ���� ���� �ð� ����

	// ������ ���
	float Alpha = FMath::Clamp(ElapsedTime / time, 0.0f, 1.0f);
	FVector NewScale = FMath::Lerp(FVector(1.0f), FVector(0.0f), Alpha);

	// ���ο� ������ ����
	ParryNiagaraComponent->SetWorldScale3D(NewScale);

	// ���� ����
	if (ElapsedTime >= time)
	{
		NAParryUpdateEnd();
	}
}

void APGNpcCharacter::NAParryUpdateEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(NAScaleTimerHandle);
	ElapsedTime = 0.0f; // ����
	
	ParryNiagaraComponent->Deactivate();
}








