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
	//�ʱ� ����  - ĸ�� /�����Ʈ ������Ʈ
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	//���ӷ�
	//������ 2048
	GetCharacterMovement()->MaxAcceleration = 1500.0f;

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	//GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Mesh ��ġ�� ȸ�� ���� �ȸ¾Ƽ�
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

	//Combo ���� �־���
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

	

	//StatComponent �߰�
	StatComponent = CreateDefaultSubobject<UPGStatComponent>(TEXT("STAT"));

	//Attack ������Ʈ �߰�
	AttackComponent = CreateDefaultSubobject<UPGAttackComponent>(TEXT("ATTACK_COMP"));

	//widget ������Ʈ �߰�
	HpBarWidgetComponent = CreateDefaultSubobject<UPGWidgetComponent>(TEXT("Widget"));
	HpBarWidgetComponent->SetupAttachment(GetMesh());
	HpBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 190.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget>HpBarWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/BP_HpBar.BP_HpBar_C'"));
	if (HpBarWidgetClass.Class)
	{
		HpBarWidgetComponent->SetWidgetClass(HpBarWidgetClass.Class);
		HpBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		// ���� ũ�� ���⼭ ���� ( �����ٶ� ũ��)s
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
		//���� ����
		StatComponent->OnHpChanged.AddUObject(HpBarWidget, &UPGHPBarWidget::UpdateHpBar);

	}
	
}

void APGBaseCharacter::AttackHitCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackHitCheck"));
	//hit ������� �޾ƿ��� ����
	FHitResult OutHitResult;
	//�Ķ����
	//traceTag ( �ݸ��� �м��Ҷ� �ĺ��� ����) , ������ ������ �浹ü�� �������� (�ö󰥶�) , ������ ���͵� 
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	//10�� ������ - ���� �����Ϳ��� �����ͼ� ����
	//���� ũ�� ����
	//const float AttackRange = StatComponent->GetBaseStat().AttackRange;
	const float AttackRange = 100.0f;
	//12�� AI ���� ������ ���� 
	const float AttackRadius = 100.0f;
	const float AttackDamage = StatComponent->GetBaseStat().Attack;
	//��ü ���� ��������->������
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	//  CCHANNEL_ABACTION �츮�� �߰��� Physics �� ��ũ��
	bool HitDetected =
		GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRadius), Params);

	if (HitDetected)
	{
		//������� �߰� �ؾ���
		//(6�� �״� ���) ������ ������ ������ �� �ִٳ�
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}


	//Debug Draw  ���ݹ��� �˾ƺ��� ���� �׷�����
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

	//7������ �߰�
	StatComponent->Damaged(DamageAmount);

	// ���� ������ ��
	return DamageAmount;
}

