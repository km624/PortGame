// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
//#include "Engine/LocalPlayer.h"
#include "PGCharacterData.h"
#include "Components/TimelineComponent.h"
#include "UI/PGHudWidget.h"
#include "Component/PGStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PortGame/PortGame.h"
#include "MotionWarpingComponent.h"
#include "Component/TargetingComponent.h"
#include "Component/PGAttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PostProcessComponent.h"






APGPlayerCharacter::APGPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting_Comp"));
	
	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess_Comp"));
	PostProcessComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UPGCharacterData> CharacterBaseData
	(TEXT("/Script/PortGame.PGCharacterData'/Game/PortGame/Input/PG_InputBaseData.PG_InputBaseData'"));
	if (CharacterBaseData.Object)
		ControlDataManager.Add(EControlData::Base, CharacterBaseData.Object);

	static ConstructorHelpers::FObjectFinder<UPGCharacterData> CharacterAimData
	(TEXT("/Script/PortGame.PGCharacterData'/Game/PortGame/Input/PG_InputAimData.PG_InputAimData'"));
	if (CharacterAimData.Object)
		ControlDataManager.Add(EControlData::Aim, CharacterAimData.Object);
	
	CurrentControlData = EControlData::Base;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	static ConstructorHelpers::FObjectFinder<UInputAction> Jump(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Jump.IA_Jump'"));
	if (Jump.Object)
	{
		JumpAction = Jump.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> Move(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Move.IA_Move'"));
	if (Move.Object)
	{
		MoveAction = Move.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> Look(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Look.IA_Look'"));
	if (Look.Object)
	{
		LookAction = Look.Object;
	}


	static ConstructorHelpers::FObjectFinder<UInputAction> Attack(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Attack.IA_Attack'"));
	if (Attack.Object)
	{
		AttackAction = Attack.Object;
	}

	//에임액션 
	static ConstructorHelpers::FObjectFinder<UInputAction> Aim(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Aim.IA_Aim'"));
	if (Aim.Object)
	{
		AimAction = Aim.Object;
	}

	//장전 액션
	static ConstructorHelpers::FObjectFinder<UInputAction> Reload(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Reload.IA_Reload'"));
	if (Reload.Object)
	{
		ReloadAction = Reload.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> Targeting(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Targetaing.IA_Targetaing'"));
	if (Targeting.Object)
	{
		TargetAction = Targeting.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SideTargeting(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_ChangeTarget.IA_ChangeTarget'"));
	if (SideTargeting.Object)
	{
		TargetSideAction = SideTargeting.Object;
	}

	Tags.Add(TAG_PLAYER);

	
}

void APGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterData(CurrentControlData);

	//임시로
	StatComponent->SetCurrentRarity(TEXT("Normal"));
	StatComponent->SetHitGauge(GetTotalStat().HitGauge);
	StatComponent->SetHp(GetTotalStat().MaxHp);

	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, FName("AimUpdate")); 
	AimTimeline.AddInterpFloat(AimCurve, TimelineProgress);


}

//인풋 매핑 - 액션에 함수 바인딩
void APGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	// 인풋 매핑이 있다면
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APGPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APGPlayerCharacter::SetNoneMove);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APGPlayerCharacter::Look);
		
		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APGPlayerCharacter::Attack);
		if(CharacterType ==EPlayerCharacterType::BlueArchive || CharacterType == EPlayerCharacterType::Nikke)
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Ongoing, this, &APGPlayerCharacter::OnGoingAttack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &APGPlayerCharacter::ReleasedAttack);
		//Aiming
		//CharacterType에 따라 바인딩 
		if (CharacterType == EPlayerCharacterType::BlueArchive || CharacterType == EPlayerCharacterType::Nikke)
		{
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &APGPlayerCharacter::PressAim);
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Ongoing, this, &APGPlayerCharacter::OnGoingAim);
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APGPlayerCharacter::ReleasedAim);

			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &APGPlayerCharacter::PressReload);
		}

		EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Started, this, &APGPlayerCharacter::FindClosestEnemyToComp);

		EnhancedInputComponent->BindAction(TargetSideAction, ETriggerEvent::Started, this, &APGPlayerCharacter::FindSideEnemyToComp);
	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void APGPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTimeline.TickTimeline(DeltaTime);
	if (TargetingComponent->GetbIsTargetLock())
	{
		TargetingComponent->TargetLockOn(DeltaTime);
	}
	else
	{
		if (bIsAttackRotation&& bIsMoving)
			SetAttackRotation(DeltaTime);

	}
	
	
}

void APGPlayerCharacter::SetCharacterData(EControlData DataName)
{
	UPGCharacterData* NewCharacterData = ControlDataManager[DataName];
	ensure(NewCharacterData);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();


			Subsystem->AddMappingContext(NewCharacterData->InputMappingContext, 0);

		}

	}
	CurrentControlData = DataName;

	//CharacterData 대로 세팅 해주기

		// Pawn
	bUseControllerRotationYaw = NewCharacterData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = NewCharacterData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = NewCharacterData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = NewCharacterData->RotationRate;
	GetCharacterMovement()->MaxWalkSpeed = NewCharacterData->MaxWalkSpeed;
	GetCharacterMovement()->JumpZVelocity = NewCharacterData->JumpZVelocity;
	GetCharacterMovement()->AirControl = NewCharacterData->AirControl;

	//스프링 암
	
	SpringArm->TargetArmLength = NewCharacterData->TargetArmLength;
	SpringArm->SetRelativeRotation(NewCharacterData->RelativeRotation);
	SpringArm->bUsePawnControlRotation = NewCharacterData->bUsePawnControlRotation;
	SpringArm->bInheritPitch = NewCharacterData->bInheritPitch;
	SpringArm->bInheritYaw = NewCharacterData->bInheritYaw;
	SpringArm->bInheritRoll = NewCharacterData->bInheritRoll;
	SpringArm->bDoCollisionTest = NewCharacterData->bDoCollisionTest;
	
	SpringArm->SetRelativeTransform(NewCharacterData->transform);

}


void APGPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
		
		bIsMoving = true;

		if (!TargetingComponent->GetbIsTargetLock())
		{
			FVector CurrentLocation = GetActorLocation();
			FVector MovementDirection = (ForwardDirection * MovementVector.X) + (RightDirection * MovementVector.Y);
			AttackVector = MovementDirection;	
		}
		
	}
}

void APGPlayerCharacter::SetNoneMove()
{
	bIsMoving = false;

}

void APGPlayerCharacter::Look(const FInputActionValue& Value)
{


	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APGPlayerCharacter::Attack()
{
	if (bIsSlow)return;

	if (bIsAim)
	{
		bIsShoot = true;
	}

	AttackToComponent();
	
	
	
}

void APGPlayerCharacter::OnGoingAttack()
{
	if (bIsAim)
	{
		
		OnbIsShoot.Broadcast(bIsShoot);
	}
}

void APGPlayerCharacter::ReleasedAttack()
{
	if (bIsAim)
	{
		bIsShoot = false;
		
		OnbIsShoot.Broadcast(bIsShoot);
		
	}

}


void APGPlayerCharacter::PressAim()
{
	if (bIsSlow)return;

	bIsAim = true;
	OnbIsAim.Broadcast(bIsAim);

	SetCharacterData(EControlData::Aim);
	AimTimeline.PlayFromStart();

	AimLocation = Camera->GetComponentLocation();
	

}

void APGPlayerCharacter::OnGoingAim()
{
	
		AimLocation = Camera->GetComponentLocation();
}

void APGPlayerCharacter::ReleasedAim()
{
	
		bIsAim = false;
		OnbIsAim.Broadcast(bIsAim);

		bIsShoot = false;
		OnbIsShoot.Broadcast(bIsShoot);
		SetCharacterData(EControlData::Base);
		AimTimeline.Reverse();
	

}

void APGPlayerCharacter::PressReload()
{
	if (bIsSlow) return;
	ReloadToWeapon();
}



void APGPlayerCharacter::AimUpdate(float deltaTime)
{
	
	float AimX = FMath::Lerp(0, 150, deltaTime);
	float AimY = FMath::Lerp(0, 50, deltaTime);

	Camera->SetRelativeLocation(FVector(AimX, AimY, 0.0f));
	
}

float APGPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (AttackComponent->GetbIsGodMode())return DamageAmount;
	if (DamageCauser->ActorHasTag(TAG_ENEMY))
	{
		StatComponent->Damaged(DamageAmount);
	}
	return DamageAmount;
}


void APGPlayerCharacter::SetUpHudWidget(UPGHudWidget* hudWidget)
{
	if (hudWidget)
	{
		hudWidget->SetUpWaidget(StatComponent->GetBaseStat(), StatComponent->GetModifierStat(), StatComponent->GetMaxHitGauge());
		//UE_LOG(LogTemp, Warning, TEXT("hudwidget %f"), StatComponent->GetCurrentHp());
		hudWidget->UpdateHpBar(StatComponent->GetCurrentHp());
		hudWidget->UpdateHitGaugeBar(StatComponent->GetCurrentHitGauge());

		//델리게이트 바인딩
		StatComponent->OnStatChanged.AddUObject(hudWidget, &UPGHudWidget::SetUpWaidget);
		StatComponent->OnHpChanged.AddUObject(hudWidget, &UPGHudWidget::UpdateHpBar);
		StatComponent->OnHitGaugeChanged.AddUObject(hudWidget, &UPGHudWidget::UpdateHitGaugeBar);

		OnbIsAim.AddUObject(hudWidget, &UPGHudWidget::CorssHairEnable);
	}
}

void APGPlayerCharacter::FindClosestEnemyToComp()
{
	TargetingComponent->SetTargetLock();
}

void APGPlayerCharacter::FindSideEnemyToComp(const struct FInputActionValue& Value)
{
	
	float direction = Value.Get<float>();
	TargetingComponent->SetSideTargetLock(direction);
	
	
}


void APGPlayerCharacter::SetMotionWarpingLocation(FVector targetPos)
{
	if (TargetingComponent->GetbIsTargetLock())
	{
		if (TargetingComponent->CharcterTargetDistance() > 300.0f)
		{
			MotionWarpingComponent->RemoveWarpTarget(TEXT("Target"));
			return;
		}

	}
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(TEXT("Target"), targetPos);
	
}

void APGPlayerCharacter::SetAttackRotation(float dt)
{
	FRotator CurrentRotation = GetActorRotation();
	FRotator TargetRotation = AttackVector.Rotation();

	// RotationSpeed에 따라 회전 속도를 조절
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, dt, 7.0f);

	SetActorRotation(NewRotation);
}

void APGPlayerCharacter::SetbIsSlowMotion(bool slowmotion)
{
	bIsSlow = slowmotion;
}

void APGPlayerCharacter::OnParryPostPorcess(bool effect)
{
	if (effect)
	{
	PostProcessComponent->Settings.bOverride_DepthOfFieldSensorWidth = true;
	PostProcessComponent->Settings.bOverride_DepthOfFieldFocalDistance = true;
	PostProcessComponent->Settings.DepthOfFieldFocalDistance =300.0f;
	PostProcessComponent->Settings.DepthOfFieldSensorWidth = 750.0f;

	PostProcessComponent->Settings.bOverride_SceneFringeIntensity = true;
	PostProcessComponent->Settings.SceneFringeIntensity = 2.5f;
	
	
	}
	else
	{
		PostProcessComponent->Settings.bOverride_DepthOfFieldSensorWidth = false;
		PostProcessComponent->Settings.bOverride_DepthOfFieldFocalDistance = false;

		PostProcessComponent->Settings.bOverride_SceneFringeIntensity = false;
	}
}









