// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/ComboData.h"
#include "Animation/AnimMontage.h"
#include "Component/PGStatComponent.h"
#include "Component/PGAttackComponent.h"
#include "PortGame/PortGame.h"
#include "UI/PGHPBarWidget.h"
#include "Component/PGWidgetComponent.h"
#include "Engine/DamageEvents.h"


// Sets default values
APGBaseCharacter::APGBaseCharacter()
{
	//초기 셋팅  - 캡슐 /무브먼트 컴포넌트
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	//가속력
	//원래값 2048
	GetCharacterMovement()->MaxAcceleration = 1500.0f;

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	//GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Mesh 위치랑 회전 해줌 안맞아서
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PGCapsule"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeletal(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (Skeletal.Object)
	{
		GetMesh()->SetSkeletalMesh(Skeletal.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/PortGame/Animation/ABP_Animation.ABP_Animation_C'"));
	if (AnimInstance.Class)

	{
		GetMesh()->SetAnimInstanceClass(AnimInstance.Class);
	}
	//Attack 컴포넌트 추가
	AttackComponent = CreateDefaultSubobject<UPGAttackComponent>(TEXT("ATTACK_COMP"));
	//StatComponent 추가
	StatComponent = CreateDefaultSubobject<UPGStatComponent>(TEXT("STAT"));



	//widget 컴포넌트 추가
	HpBarWidgetComponent = CreateDefaultSubobject<UPGWidgetComponent>(TEXT("Widget"));
	HpBarWidgetComponent->SetupAttachment(GetMesh());
	HpBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 190.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget>HpBarWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/BP_HpBar.BP_HpBar_C'"));
	if (HpBarWidgetClass.Class)
	{
		HpBarWidgetComponent->SetWidgetClass(HpBarWidgetClass.Class);
		HpBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		// 위젯 크기 여기서 지정 ( 가느다란 크기)s
		HpBarWidgetComponent->SetDrawSize(FVector2D(200.0f, 40.0f));

		HpBarWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetClassFail"));
	}
	
	//hitMontage
	static ConstructorHelpers::FObjectFinder<class UAnimMontage>
		MONTAGE_HIT(TEXT("/Script/Engine.AnimMontage'/Game/PortGame/Animation/Base/HitMontage.HitMontage'"));
	if (MONTAGE_HIT.Object)
	{
		HitMontage = MONTAGE_HIT.Object;
	}
	static ConstructorHelpers::FObjectFinder<class UAnimMontage>
		MONTAGE_DEAD(TEXT("/Script/Engine.AnimMontage'/Game/PortGame/Animation/Base/DeadSMontage.DeadSMontage'"));
	if (MONTAGE_DEAD.Object)
	{
		DeadMontage = MONTAGE_DEAD.Object;
	}

}

void APGBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	StatComponent->OnHitGaugeZero.AddUObject(this, &ThisClass::PlayHitMontage);

	StatComponent->OnHpZero.AddUObject(this, &ThisClass::SetDead);
}

void APGBaseCharacter::AttackToComponent()
{
	AttackComponent->AttackToWeapon();
	
}

float APGBaseCharacter::ReturnAimOffset()
{
	FRotator rtemp = GetActorRotation() - GetBaseAimRotation();

	rtemp.Normalize();
	float Direction = rtemp.Pitch;
	AimOffset = FMath::Clamp(Direction, -55.0f, 55.0f);
	return AimOffset;
}



void APGBaseCharacter::ReloadToWeapon()
{
	bIsReload = true;
	OnbIsReload.Broadcast();
}

FPGCharacterStat APGBaseCharacter::GetTotalStat()
{
	return StatComponent->GetTotalStat();
}

void APGBaseCharacter::SetUpBaseStat(FName baseStat)
{
	StatComponent->SetBaseStat(baseStat);
}

void APGBaseCharacter::SetUpModifierStat(FPGCharacterStat ModiferStat)
{
	StatComponent->SetModifierStat(ModiferStat);
}


void APGBaseCharacter::HiddenWidget()
{
	HpBarWidgetComponent->SetVisibility(false);
}

void APGBaseCharacter::SetUpHpWidget(UPGUserWidget* InUserWidget)
{
	UPGHPBarWidget* HpBarWidget = Cast<UPGHPBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetUpWaidget(StatComponent->GetBaseStat(), StatComponent->GetModifierStat(),StatComponent->GetMaxHitGauge());
		HpBarWidget->UpdateHpBar(StatComponent->GetCurrentHp());
		HpBarWidget->UpdateHitGaugeBar(StatComponent->GetCurrentHitGauge());

		//위젯 영역
		StatComponent->OnHpChanged.AddUObject(HpBarWidget, &UPGHPBarWidget::UpdateHpBar);
		StatComponent->OnHitGaugeChanged.AddUObject(HpBarWidget, &UPGHPBarWidget::UpdateHitGaugeBar);
		StatComponent->OnStatChanged.AddUObject(HpBarWidget, &UPGHPBarWidget::SetUpWaidget);

	}
	
}

void APGBaseCharacter::AttackHitCheck()
{
	//UE_LOG(LogTemp, Warning, TEXT("AttackHitCheck"));
	////hit 결과값을 받아오는 변수
	//FHitResult OutHitResult;
	////파라미터
	////traceTag ( 콜리전 분석할때 식별자 정보) , 복잡한 형태의 충돌체도 감지할지 (올라갈때) , 무시할 액터들 
	//FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	////10강 데이터 - 스텟 데이터에서 가져와서 적용
	////도형 크기 설정
	////const float AttackRange = StatComponent->GetBaseStat().AttackRange;
	//const float AttackRange = StatComponent->GetTotalStat().AttackRange;
	////12강 AI 공격 구현을 위한 
	//const float AttackRadius = StatComponent->GetTotalStat().AttackRange;
	//const float AttackDamage = StatComponent->GetTotalStat().Attack;
	////구체 날릴 시작지점->끝지점
	//const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	//const FVector End = Start + GetActorForwardVector() * AttackRange;

	////  CCHANNEL_ABACTION 우리가 추가한 Physics 의 매크로
	//bool HitDetected =
	//	GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRadius), Params);

	//if (HitDetected)
	//{
	//	//헤더파일 추가 해야함
	//	//(6강 죽는 모션) 데미지 종류를 지정할 수 있다네
	//	FDamageEvent DamageEvent;
	//	OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	//}

	
	TArray<FHitResult> OutHitResults;

	// 충돌 쿼리 파라미터 설정
	// traceTag: 충돌 분석 시 식별자 정보
	// bTraceComplex: 복잡한 충돌체도 감지할지 여부
	// IgnoreActor: 충돌 체크 시 무시할 액터
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	// 스탯 컴포넌트에서 공격 범위, 반경, 데미지 값을 가져옴
	const float AttackRange = StatComponent->GetTotalStat().AttackRange;
	const float AttackRadius = StatComponent->GetTotalStat().AttackRange;
	const float AttackDamage = StatComponent->GetTotalStat().Attack;

	// 공격 시작 지점 계산: 현재 액터의 위치에서 전방 벡터와 캡슐 반경을 이용
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();

	// 공격 끝 지점 계산: 시작 지점에서 전방 벡터 방향으로 공격 범위 만큼 이동
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	// SweepMultiByChannel을 사용하여 충돌 감지 수행
	bool HitDetected = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	// 히트가 감지된 경우 처리
	if (HitDetected)
	{
		// 히트된 모든 결과를 순회
		for (const FHitResult& Hit : OutHitResults)
		{
			// 히트된 액터가 유효한지 확인
			if (Hit.GetActor())
			{
				// 데미지 이벤트 생성
				FDamageEvent DamageEvent;

				// 히트된 액터에 데미지 적용
				Hit.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
			}
		}
	}


	//Debug Draw  공격범위 알아보기 위한 그려보기
//#if ENABLE_DRAW_DEBUG
//
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//	float CapsuleHalfHeight = AttackRange * 0.5f;
//	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//
//	//DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
//
//	FVector TraceVec = GetActorForwardVector() * AttackRange;
//	FVector Center = GetActorLocation() + TraceVec * 0.5f;
//	float HalfHeight = AttackRange * 0.5f + AttackRadius;
//	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
//	//FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//	float DebugLifeTime = 5.0f;
//
//	DrawDebugCapsule(GetWorld(),
//		Center,
//		HalfHeight,
//		AttackRadius,
//		CapsuleRot,
//		DrawColor,
//		false,
//		DebugLifeTime);
//
//
//#endif

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	float DebugLifeTime = 5.0f;

	// SweepMulti와 동일한 범위를 그려줌
	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif
}

float APGBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//7강에서 추가
	StatComponent->Damaged(DamageAmount);

	// 받을 데미지 값
	return DamageAmount;
}

void APGBaseCharacter::PlayHitMontage()
{
	//맞는 애니메이션
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(HitMontage, 1.0f);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APGBaseCharacter::HitMontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, HitMontage);

}

void APGBaseCharacter::HitMontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	//StatComponent->ResetHitGauge();
}

void APGBaseCharacter::SetDead()
{
	//이동 기능 제한
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//콤보 도중 죽을 수 있으니까 다른 몽타주 멈춤
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);

	
	SetActorEnableCollision(false);
	
	HpBarWidgetComponent->SetHiddenInGame(true);
}


