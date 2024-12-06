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
#include "Kismet/GameplayStatics.h"
#include "Physics/PGCollision.h"
#include "MotionWarpingComponent.h"
#include "Interface/AIControllerInterface.h"
#include "PortGame/PortGame.h"
#include "NiagaraComponent.h"   
#include "NiagaraSystem.h"  


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

	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PGCAPSULE);

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
	//Attack ������Ʈ �߰�
	AttackComponent = CreateDefaultSubobject<UPGAttackComponent>(TEXT("ATTACK_COMP"));
	//StatComponent �߰�
	StatComponent = CreateDefaultSubobject<UPGStatComponent>(TEXT("STAT"));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion_Warp"));

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

	//���̾ư���
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);

}

void APGBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	StatComponent->OnHitGaugeZero.AddUObject(this, &ThisClass::PlayHitMontage);

	StatComponent->OnHpZero.AddUObject(this, &ThisClass::SetDead);

	//SetPlayerCharacterType();
	NiagaraComponent->OnSystemFinished.AddDynamic(this, &APGBaseCharacter::OnNiagaraSystemFinished);


	//�� ����

	SetGenericTeamId(TeamId);

	/*FGenericTeamId currentteam = GetGenericTeamId();
	SLOG(TEXT("actor : %s , myteamide : %d"), *GetActorLabel(), currentteam.GetId());*/
}

void APGBaseCharacter::AttackToComponent()
{
	//�׷α�� ����x
	if (bIsGroggy) return;
	AttackComponent->AttackToWeapon();
	
}

void APGBaseCharacter::ComboCheckStartToComp()
{
	AttackComponent->ComboCheckStart();
}


void APGBaseCharacter::ComboCheckEndToComp()
{
	AttackComponent->CombocheckEnd();
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

		//���� ����
		StatComponent->OnHpChanged.AddUObject(HpBarWidget, &UPGHPBarWidget::UpdateHpBar);
		StatComponent->OnHitGaugeChanged.AddUObject(HpBarWidget, &UPGHPBarWidget::UpdateHitGaugeBar);
		StatComponent->OnStatChanged.AddUObject(HpBarWidget, &UPGHPBarWidget::SetUpWaidget);

	}
	
}

void APGBaseCharacter::AttackHitCheckToComp()
{
	if (bIsGroggy)return;
	AttackComponent->AttackHitCheck();

}

float APGBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return DamageAmount;
}

void APGBaseCharacter::PlayHitMontage()
{
	if (bIsDead)
		return;
	
	GetController()->SetIgnoreMoveInput(true);
	bIsGroggy = true;
	
	IAIControllerInterface* AIController = Cast<IAIControllerInterface>(GetController());
	if (AIController)
	{
		AIController->StopTree();
	}
	//�´� �ִϸ��̼�
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(HitMontage, 1.0f);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APGBaseCharacter::HitMontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, HitMontage);


}

void APGBaseCharacter::HitGaugeZeroEffect()
{
	if (bIsDead)
		return;
	FVector BackwardVector = -GetActorForwardVector(); 
	GetCharacterMovement()->AddImpulse(BackwardVector * 1000.0f, true);
	
}

void APGBaseCharacter::HitMontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	if (bIsDead)
		return;

	GetController()->SetIgnoreMoveInput(false);
	bIsGroggy = false;

	IAIControllerInterface* AIController = Cast<IAIControllerInterface>(GetController());
	if (AIController)
	{
		AIController->StartTree();

		//�ӽ�
		CustomTimeDilation = 1.0f;
	}
	
}

void APGBaseCharacter::SetDead()
{
	
	//�̵� ��� ����
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	bIsDead = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//�޺� ���� ���� �� �����ϱ� �ٸ� ��Ÿ�� ����
	AnimInstance->StopAllMontages(0.0f);

	AnimInstance->Montage_Play(DeadMontage, 1.0f);

	
	
	SetActorEnableCollision(false);
	
	HpBarWidgetComponent->SetHiddenInGame(true);

	//Destroy();
}

void APGBaseCharacter::StartNiagaraEffect()
{
	if (NAWeaponEffect)
	{
		NiagaraComponent->SetAsset(NAWeaponEffect);
		NiagaraComponent->Activate();  
		SLOG(TEXT("NiagiaraON"));
		// �ý����� �Ϸ�Ǿ��� �� ȣ��� ��������Ʈ ���ε�
		
	}
}

void APGBaseCharacter::OnNiagaraSystemFinished(UNiagaraComponent* FinishedComponent)
{
	SLOG(TEXT("NiagaraDone"));
	FinishedComponent->Deactivate();
}






