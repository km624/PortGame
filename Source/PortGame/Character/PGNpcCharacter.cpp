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
#include "Component/PGWidgetComponent.h"
#include "WorldPartition/WorldPartition.h"
#include "WorldPartition/WorldPartitionSubsystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BillboardComponent.h"







APGNpcCharacter::APGNpcCharacter() 
{
	

	Tags.Add(TAG_AI);

	GetCapsuleComponent()->InitCapsuleSize(20.0f, 96.0f);
	
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
	

	//bUseControllerRotationYaw = true;

	ParryNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParryEffectComp"));
	ParryNiagaraComponent->SetupAttachment(RootComponent);
	ParryNiagaraComponent->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> parryEffect(TEXT("/Script/Niagara.NiagaraSystem'/Game/PortGame/Effect/Niagara/NA_ParryState.NA_ParryState'"));
	if (parryEffect.Object)
	{
		NAParryEffect = parryEffect.Object;
	}

	bIsRendered = true;

	TargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetWidgetComponent"));

	TargetWidget->SetupAttachment(GetMesh());
	TargetWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget>TargetWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/BP_TargetLock.BP_TargetLock_C'"));
	if (TargetWidgetClass.Class)
	{
		TargetWidget->SetWidgetClass(TargetWidgetClass.Class);
		TargetWidget->SetWidgetSpace(EWidgetSpace::Screen);

		// 위젯 크기 여기서 지정 ( 가느다란 크기)s
		TargetWidget->SetDrawSize(FVector2D(30.0f, 30.0f));
		TargetWidget->SetHiddenInGame(true);
		TargetWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetClassFail"));
	}
}

void APGNpcCharacter::BeginPlay()
{
	Super::BeginPlay();

	//SetPhysicsSetting();
	
	if (CharacterType == EPlayerCharacterType::BlueArchive || CharacterType == EPlayerCharacterType::Nikke)
	{
		bIsAim = true;
		
	}
	//NPC 캐릭터 팀 색깔 설정
	ChangeNpcColor();
	currentSlowtime = 0.0f;
	
}

void APGNpcCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	GetWorld()->GetTimerManager().ClearTimer(DeadHiddentimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(NAScaleTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(NPCHitTimer);
}



void APGNpcCharacter::SetupCharacterData(UBaseCharacterDataAsset* characterdata)
{
	Super::SetupCharacterData(characterdata);

	StatComponent->SetCurrentRarity(TEXT("NPC"));
}

void APGNpcCharacter::EnableCharacter()
{
	Super::EnableCharacter();

	bIshit = false;
	bIsRendered = true;
	bIsParry = false;
	currentSlowtime = 0.0f;
	
}

void APGNpcCharacter::Tick(float deltatime)
{
	Super::Tick(deltatime);

	CheckCharacterRender();

}



	

void APGNpcCharacter::ChangeNpcColor()
{
	
	// 팀 색상 설정
	FLinearColor TeamColor = (TeamId != 1) ? FLinearColor::Red : FLinearColor::Blue;

	GetMesh()->SetCustomDepthStencilValue(TeamId);


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

void APGNpcCharacter::SetPhysicsSetting()
{
	
	GetCapsuleComponent()->SetMassOverrideInKg(NAME_None, 20.0f, true);

	
	GetCapsuleComponent()->SetLinearDamping(1.0f);

	
	GetCapsuleComponent()->BodyInstance.bLockXRotation = true; 
	GetCapsuleComponent()->BodyInstance.bLockYRotation = true; 

}


float APGNpcCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	//if (!EventInstigator->GetPawn()) return DamageAmount;
	//APGBaseCharacter* attackPawn = Cast<APGBaseCharacter>(EventInstigator->GetPawn());
	
	if (EventInstigator->GetPawn() ==NULL)return DamageAmount;

	if (DamageCauser == NULL)return DamageAmount;

	if (!GetTeamAttitudeTowards(*DamageCauser)) return DamageAmount;
	
	if (TeamId != 1)
	{
		bIshit = true;
		HpBarWidgetComponent->SetHiddenInGame(false);
		OnHited.Broadcast(bIshit);

		GetWorld()->GetTimerManager().SetTimer(
			NPCHitTimer,
			[this]() {
				bIshit = false;
				HpBarWidgetComponent->SetHiddenInGame(true);
				OnHited.Broadcast(bIshit);
				GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);
			}, WidgetShowTime, false
		);
	}
	
	HitImpulseVector *= 2.0f;
	
	AActor* AttackActor = DamageCauser;

	if (DamageCauser->ActorHasTag(TAG_GRENADE))
	{
		HitImpulseVector += (FVector(0, 0, 1) * 50.0f);
		AttackActor = EventInstigator->GetPawn();
	}

	//패리중일때
	if (bIsParry)
	{

		StatComponent->HitGaugeDamaged(GetTotalStat().HitGauge);

		NAParryUpdateEnd();
	}
	else
		StatComponent->Damaged(DamageAmount, AttackActor);
	
	
	//if (attackPawn)
	//{
	//	//적팀일시
	//	if (GetTeamAttitudeTowards(*DamageCauser) && !DamageCauser->ActorHasTag(TAG_GRENADE))
	//	{
	//		//패리중일때
	//		if (bIsParry)
	//		{

	//			StatComponent->HitGaugeDamaged(GetTotalStat().HitGauge);

	//			NAParryUpdateEnd();

	//		}
	//		else
	//			StatComponent->Damaged(DamageAmount, DamageCauser);
	//	}

	//	//수류탄에 맞았을시
	//	if (DamageCauser->ActorHasTag(TAG_GRENADE))
	//	{

	//		/*FVector Direction = GetActorLocation() - DamageCauser->GetActorLocation();
	//		Direction.Normalize();*/
	//		HitImpulseVector += (FVector(0, 0, 1) * 50.0f);

	//		if (GetTeamAttitudeTowards(*EventInstigator->GetPawn()))
	//		{
	//			StatComponent->Damaged(DamageAmount, EventInstigator->GetPawn());
	//		}
	//		
	//	}
	//}
	
	return DamageAmount;
}

void APGNpcCharacter::NPCAttackHitStop(float time)
{
	
	if (bIsDead) return;

	if (currentSlowtime >= time)return;

	GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);
	currentSlowtime = time;
	
	CustomTimeDilation = 0.01f;
	GetWorld()->GetTimerManager().SetTimer(
		NPCHitStoptimerHandle,
		[this]() {
			CustomTimeDilation = 1.0f;
			currentSlowtime = 0.0f;
			GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);
		}, time, false
	);
	
}

void APGNpcCharacter::SetDead(AActor* DamageCauser)
{
	Super::SetDead(DamageCauser);

	CommonNPCDeadLogic(DamageCauser);
	
	GetWorld()->GetTimerManager().SetTimer(
		DeadHiddentimerHandle,
		[this]() {
			
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				AnimInstance->StopAllMontages(0.0f);
			}
			//AttackComponent->ReturnWeaponPool();
			ReturnCharacterToPool();
			GetWorld()->GetTimerManager().ClearTimer(DeadHiddentimerHandle);
		}, ReturnPoolTime, false);

}

void APGNpcCharacter::CommonNPCDeadLogic(AActor* DamageCauser)
{
	CustomTimeDilation = 1.0f;

	GetWorld()->GetTimerManager().ClearTimer(NPCHitStoptimerHandle);

	NAParryUpdateEnd();

	GetWorld()->GetTimerManager().ClearTimer(NAScaleTimerHandle);

	GetWorld()->GetTimerManager().ClearTimer(NPCHitTimer);

	//자신의 필드에 데이터 내가 누가한테 죽었는지 보냄
	if (MyAIController)
	{
		IGenericTeamAgentInterface* team = Cast<IGenericTeamAgentInterface>(DamageCauser);
		int8 teamid = team->GetGenericTeamId();
		//SLOG(TEXT("AI DEAD"));
		if (teamid != 0)
			MyAIController->TOMyFieldDead(teamid);
	}
}

void APGNpcCharacter::ReturnCharacterToPool()
{
	//애니메이션 제거
	GetMesh()->SetAnimInstanceClass(nullptr);
	//무기 리턴
	AttackComponent->ReturnWeaponPool();

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
	if (TargetPawn == NULL) return;

	if (!TargetPawn->ActorHasTag(TAG_PLAYER)) return;

	if (bIsGroggy)return;

	if (RandomParry()==false) return;

	NAParryStart();
	bIsParry = true;
	CustomTimeDilation = 0.2f;
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

void APGNpcCharacter::ForceReturnObjectPool()
{
	
	GetCharacterMovement()->Deactivate();

	GetMesh()->SetAnimInstanceClass(nullptr);

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.0f);
	}
	
	ReturnCharacterToPool();
	
}

void APGNpcCharacter::CheckCharacterRender()
{
	if (GetMesh()->WasRecentlyRendered())
	{
		
		OnRenderCharacter();
		
	}
	else
	{
		
		NotRenderCharacter();
	
	}
}

void APGNpcCharacter::NotRenderCharacter()
{
	if (!bIsRendered) return;
	bIsRendered = false;
	
	MyAIController->SetVisible(bIsRendered);
	
	GetMesh()->bPauseAnims = true;
}

void APGNpcCharacter::OnRenderCharacter()
{
	if (bIsRendered) return;
	bIsRendered = true;

	MyAIController->SetVisible(bIsRendered);
	GetMesh()->bPauseAnims = false;
}

void APGNpcCharacter::SetAnimationDistanceFactor()
{
	if (GetMesh())
	{
		
		GetMesh()->bEnableUpdateRateOptimizations = true;

		
		FAnimUpdateRateParameters* Params = GetMesh()->AnimUpdateRateParams;
		
		int32 LODLevel = GetMesh()->GetPredictedLODLevel();
		

		Params->BaseVisibleDistanceFactorThesholds = { 0.5f, 0.3f, 0.1f };
	}
}

void APGNpcCharacter::SetTargeting(bool targeting)
{
	TargetWidget->SetHiddenInGame(!targeting);
	
}








