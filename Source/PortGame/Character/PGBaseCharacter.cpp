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
#include "NiagaraFunctionLibrary.h"
#include "Data/BaseCharacterDataAsset.h"
#include "Engine/StreamableManager.h"
#include "UObject/SoftObjectPath.h"
#include "Engine/AssetManager.h"



const FString APGBaseCharacter::HitMontage= TEXT("HitMontage");
const FString APGBaseCharacter::DeadMontage = TEXT("DeadMontage");


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

	GetCharacterMovement()->bUseRVOAvoidance = true; // RVO 회피 활성화
	GetCharacterMovement()->AvoidanceWeight = 0.8f; // 회피 강도
	GetCharacterMovement()->AvoidanceConsiderationRadius = 100.0f;

	// Mesh 위치랑 회전 해줌 안맞아서
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_PGCAPSULE);

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeletal(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (Skeletal.Object)
	{
		GetMesh()->SetSkeletalMesh(Skeletal.Object);
	}*/

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/PortGame/Animation/ABP_Animation.ABP_Animation_C'"));
	if (AnimInstance.Class)

	{
		GetMesh()->SetAnimInstanceClass(AnimInstance.Class);
	}
	//Attack 컴포넌트 추가
	AttackComponent = CreateDefaultSubobject<UPGAttackComponent>(TEXT("ATTACK_COMP"));
	//StatComponent 추가
	StatComponent = CreateDefaultSubobject<UPGStatComponent>(TEXT("STAT"));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion_Warp"));

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
		HpBarWidgetComponent->SetDrawSize(FVector2D(75.0f, 15.0f));
		HpBarWidgetComponent->SetHiddenInGame(true);
		HpBarWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetClassFail"));
	}

	

	//나이아가라

	BaseNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BaseNiagaraComp"));
	BaseNiagaraComponent->SetupAttachment(RootComponent);
	BaseNiagaraComponent->bAutoActivate = false;

}

void APGBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (baseCharacterData)
		SetupCharacterData(baseCharacterData);
}

void APGBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	//SLOG(TEXT("StartBeginpaly"));

	StatComponent->OnHitGaugeZero.AddUObject(this, &ThisClass::PlayHitMontage);

	StatComponent->OnHpZero.AddUObject(this, &ThisClass::SetDead);

	//나이아가라 바인딩
	BaseNiagaraComponent->OnSystemFinished.AddDynamic(this, &APGBaseCharacter::OnNiagaraSystemFinished);


	SetGenericTeamId(TeamId);


}

void APGBaseCharacter::EnableCharacter()
{
	//이동 기능 제한
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	bIsDead = false;
	bIsGroggy = false;
	bIsReload = false;
	bIsAim = false;
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	HpBarWidgetComponent->SetHiddenInGame(false);

	
}

void APGBaseCharacter::SetCharacterData(UBaseCharacterDataAsset* characterdata)
{
	baseCharacterData = characterdata; 
	//SetupCharacterData(baseCharacterData);
}

void APGBaseCharacter::SetupCharacterData(UBaseCharacterDataAsset* characterdata)
{

	CharacterType = characterdata->Charactertype;
	CharacterTypeEffect = characterdata->CharacterTypeEffect;
	GetMesh()->SetSkeletalMesh(characterdata->SkeletalMesh);
	GetMesh()->SetAnimInstanceClass(characterdata->AnimInstanceClass);

	CharacterName = characterdata->GetMeshNameAsString();

	LoadAndPlayMontageByPath(CharacterName,HitMontage);
	LoadAndPlayMontageByPath(CharacterName,DeadMontage);
	
	AttackComponent->SetupAttackData(characterdata);

	StatComponent->SetCurrentRarity(characterdata->Rarity);

	//SLOG(TEXT("BaseCharacterr : SetupCharacterEnd"));


}

void APGBaseCharacter::LoadAndPlayMontageByPath(const FString& SkeletonName, const FString& MontageName)
{
	FString AssetPath = FString::Printf(TEXT("/Game/PortGame/Character/%s/Animation/Montage/%s.%s"), *SkeletonName, *MontageName, *MontageName);
	
	// 소프트 오브젝트 경로 생성
	FSoftObjectPath MontagePath(AssetPath);
	
	// 비동기로 로드
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	StreamableManager.RequestAsyncLoad(MontagePath, FStreamableDelegate::CreateLambda([MontagePath,this, MontageName]()
		{
			UObject* LoadedObject = MontagePath.ResolveObject();
			if (UAnimMontage* Montage = Cast<UAnimMontage>(LoadedObject))
			{
				AllMontage.Add(MontageName, Montage);
				//SLOG(TEXT("MontageLoad : %s"), *MontagePath.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to load montage: %s"), *MontagePath.ToString());
			}
		}));
}



void APGBaseCharacter::AttackToComponent()
{
	//그로기면 공격x
	if (bIsGroggy) return;

	if (bIsUltiSkill)return;


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

void APGBaseCharacter::AddUltiSkillGaugeToComp(float addUlitSkill)
{
	StatComponent->AddUlitSkillGauge(addUlitSkill);
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
		HpBarWidget->SetUpWidget(StatComponent->GetBaseStat(), StatComponent->GetModifierStat());
		HpBarWidget->UpdateHpBar(StatComponent->GetCurrentHp());
		HpBarWidget->UpdateHitGaugeBar(StatComponent->GetCurrentHitGauge());

		//위젯 영역
		StatComponent->OnHpChanged.AddUObject(HpBarWidget, &UPGHPBarWidget::UpdateHpBar);
		StatComponent->OnHitGaugeChanged.AddUObject(HpBarWidget, &UPGHPBarWidget::UpdateHitGaugeBar);
		StatComponent->OnStatChanged.AddUObject(HpBarWidget, &UPGHPBarWidget::SetUpWidget);

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

	HitImpulseVector = -GetActorForwardVector() * 1000.0f;

	return DamageAmount;
}

void APGBaseCharacter::PlayHitMontage()
{
	if (bIsDead)
		return;

	if (GetController() == NULL)
		return;

	GetController()->SetIgnoreMoveInput(true);
	bIsGroggy = true;

	IAIControllerInterface* AIController = Cast<IAIControllerInterface>(GetController());
	if (AIController)
	{
		AIController->StopTree();
	}
	//맞는 애니메이션
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(AllMontage[HitMontage], 1.0f);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APGBaseCharacter::HitMontageEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AllMontage[HitMontage]);


}

void APGBaseCharacter::HitGaugeZeroEffect()
{
	if (bIsDead)
		return;

	GetCharacterMovement()->AddImpulse(HitImpulseVector, true);

}

void APGBaseCharacter::HitMontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	if (bIsDead)
		return;

	if (GetController() == NULL)
		return;

	GetController()->SetIgnoreMoveInput(false);
	bIsGroggy = false;

	IAIControllerInterface* AIController = Cast<IAIControllerInterface>(GetController());
	if (AIController)
	{
		AIController->StartTree();

		
	}

}

void APGBaseCharacter::SetDead(int8 teamid)
{

	//이동 기능 제한
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	bIsDead = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//콤보 도중 죽을 수 있으니까 다른 몽타주 멈춤
	AnimInstance->StopAllMontages(0.0f);

	AnimInstance->Montage_Play(AllMontage[DeadMontage], 1.0f);

	OnNiagaraSystemFinished(BaseNiagaraComponent);

	SetActorEnableCollision(false);

	HpBarWidgetComponent->SetHiddenInGame(true);


}

void APGBaseCharacter::SetteamId(uint8 teamId)
{
	TeamId = teamId;
	SetGenericTeamId(TeamId);
}

void APGBaseCharacter::StartNiagaraEffect(UNiagaraSystem* niagara, FVector TargetLocation)
{
	if (niagara)
	{
		if (BaseNiagaraComponent->IsActive())
			BaseNiagaraComponent->Deactivate();
		BaseNiagaraComponent->SetAsset(niagara);
		BaseNiagaraComponent->SetWorldLocation(TargetLocation);
		BaseNiagaraComponent->Activate();


	}
}

void APGBaseCharacter::OnNiagaraSystemFinished(UNiagaraComponent* FinishedComponent)
{
	//SLOG(TEXT("NiagaraEND"));
	FinishedComponent->Deactivate();

}

void APGBaseCharacter::SkillToComponent()
{
	if (bIsUltiSkill)return;
	AttackComponent->SkillAttack();
}

void APGBaseCharacter::SetbIsNikkeSkill(bool skill)
{
	if (skill)
	{
		GetCharacterMovement()->DisableMovement();
		AttackComponent->SetbIsGodMode(true);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		AttackComponent->SetbIsGodMode(false);
	}
	bIsNikkeSkill = skill;
}

void APGBaseCharacter::UltimateSkillToComponent()
{
	if (StatComponent->GetCurrentUltiSkillGauge() >= StatComponent->GetMaxUltiSkillGauge())
	{

		AttackComponent->UltiSkillAttack();
		if (CharacterTypeEffect)
		{
			FVector effectlocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 95.0f);
			StartNiagaraEffect(CharacterTypeEffect, effectlocation);
		}
		StatComponent->ResetUlitSkillGauge();
	}

}

void APGBaseCharacter::SetbIsUltiSkill(bool bisulti)
{
	bIsUltiSkill = bisulti;
	if (GetController() == NULL)
		return;
	if (!bIsUltiSkill)
	{

		GetController()->SetIgnoreMoveInput(false);
		IAIControllerInterface* AIController = Cast<IAIControllerInterface>(GetController());
		if (AIController)
		{
			AIController->StartTree();
		}
	}
	else
	{

		GetController()->SetIgnoreMoveInput(true);
		IAIControllerInterface* AIController = Cast<IAIControllerInterface>(GetController());
		if (AIController)
		{
			AIController->StopTree();
		}

	}
}









