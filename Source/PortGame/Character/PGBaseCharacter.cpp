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

	//Combo 값들 넣어줌
	/*static ConstructorHelpers::FObjectFinder<UAnimMontage>comboMontage(TEXT("/Script/Engine.AnimMontage'/Game/PortGame/Animation/Combo/ComboMontage1.ComboMontage1'"));
	if (comboMontage.Object)
	{
		ComboMontage = comboMontage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UComboData>comboData(TEXT("/Script/PortGame.ComboData'/Game/PortGame/Animation/Combo/ComboData.ComboData'"));
	if (comboData.Object)
	{
		ComboData = comboData.Object;
	}*/

	

	//StatComponent 추가
	StatComponent = CreateDefaultSubobject<UPGStatComponent>(TEXT("STAT"));

	//Attack 컴포넌트 추가
	AttackComponent = CreateDefaultSubobject<UPGAttackComponent>(TEXT("ATTACK_COMP"));

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
		HpBarWidgetComponent->SetDrawSize(FVector2D(200.0f, 50.0f));

		HpBarWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetClassFail"));
	}
	
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

void APGBaseCharacter::SetUpHpWidget(UPGUserWidget* InUserWidget)
{
	UPGHPBarWidget* HpBarWidget = Cast<UPGHPBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetUpWaidget(StatComponent->GetBaseStat());
		HpBarWidget->UpdateHpBar(StatComponent->GetCurrentHp());
		//위젯 영역
		StatComponent->OnHpChanged.AddUObject(HpBarWidget, &UPGHPBarWidget::UpdateHpBar);

	}
	
}

void APGBaseCharacter::AttackHitCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackHitCheck"));
	//hit 결과값을 받아오는 변수
	FHitResult OutHitResult;
	//파라미터
	//traceTag ( 콜리전 분석할때 식별자 정보) , 복잡한 형태의 충돌체도 감지할지 (올라갈때) , 무시할 액터들 
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	//10강 데이터 - 스텟 데이터에서 가져와서 적용
	//도형 크기 설정
	//const float AttackRange = StatComponent->GetBaseStat().AttackRange;
	const float AttackRange = 100.0f;
	//12강 AI 공격 구현을 위한 
	const float AttackRadius = 100.0f;
	const float AttackDamage = StatComponent->GetBaseStat().Attack;
	//구체 날릴 시작지점->끝지점
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	//  CCHANNEL_ABACTION 우리가 추가한 Physics 의 매크로
	bool HitDetected =
		GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRadius), Params);

	if (HitDetected)
	{
		//헤더파일 추가 해야함
		//(6강 죽는 모션) 데미지 종류를 지정할 수 있다네
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}


	//Debug Draw  공격범위 알아보기 위한 그려보기
#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	//DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	//FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

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

