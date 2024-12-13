// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PGCharacterData.h"
#include "Components/TimelineComponent.h"
#include "UI/PGHudWidget.h"
#include "Component/PGStatComponent.h"
#include "Components/CapsuleComponent.h"
//#include "Kismet/KismetMathLibrary.h"
#include "PortGame/PortGame.h"
#include "MotionWarpingComponent.h"
#include "Component/TargetingComponent.h"
#include "Component/PGAttackComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "Components/PostProcessComponent.h"
#include "Animation/AnimMontage.h"
#include "Physics/PGCollision.h"
#include "Engine/OverlapResult.h"
#include "Interface/NPCParryCheckInterface.h"
#include "Interface/PlayerCameraShakeInterface.h"
#include "LevelSequenceActor.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "Skill/SkillBase.h"
#include "Player/PGPlayerController.h"
#include "Weapon/Rifle.h"


APGPlayerCharacter::APGPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting_Comp"));
	
	//PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess_Comp"));
	//PostProcessComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UPGCharacterData> CharacterBaseData
	(TEXT("/Script/PortGame.PGCharacterData'/Game/PortGame/Input/PG_InputBaseData.PG_InputBaseData'"));
	if (CharacterBaseData.Object)
		ControlDataManager.Add(EControlData::Base, CharacterBaseData.Object);

	static ConstructorHelpers::FObjectFinder<UPGCharacterData> CharacterAimData
	(TEXT("/Script/PortGame.PGCharacterData'/Game/PortGame/Input/PG_InputAimData.PG_InputAimData'"));
	if (CharacterAimData.Object)
		ControlDataManager.Add(EControlData::Aim, CharacterAimData.Object);
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DashMon(TEXT("/Script/Engine.AnimMontage'/Game/PortGame/Animation/Base/DashMontage.DashMontage'"));
	if (DashMon.Object)
	{
		DashMontage = DashMon.Object;
	}

	CurrentControlData = EControlData::Base;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetActive(true);

	CutSceneCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CutSceneCamera"));
	CutSceneCamera->SetupAttachment(RootComponent);
	CutSceneCamera->SetActive(false);

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

	static ConstructorHelpers::FObjectFinder<UInputAction> Dash(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Dash.IA_Dash'"));
	if (Dash.Object)
	{
		DashAction = Dash.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> EvadeLeft(TEXT("/Script/Engine.AnimMontage'/Game/PortGame/Animation/Base/EvadeLeftMontage.EvadeLeftMontage'"));
	if (EvadeLeft.Object)
	{
		LeftEvadeMontage = EvadeLeft.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EvadeRight(TEXT("/Script/Engine.AnimMontage'/Game/PortGame/Animation/Base/EvadeRightMontage.EvadeRightMontage'"));
	if (EvadeRight.Object)
	{
		RightEvadeMontage =EvadeRight.Object;
	}

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> EvadeCameraShake(TEXT("/Script/Engine.Blueprint'/Game/PortGame/Effect/CameraShake/EvadeCameraShake.EvadeCameraShake_C'"));
	if (EvadeCameraShake.Class)
	{
		EvadeCameraShakeClass = EvadeCameraShake.Class;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> Skill(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Skill.IA_Skill'"));
	if (Skill.Object)
	{
		SkillAction = Skill.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> Ulti(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_UltiSkill.IA_UltiSkill'"));
	if (Ulti.Object)
	{
		UltiSkillAction = Ulti.Object;
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

	FGenericTeamId currentteam = GetGenericTeamId();
	SLOG(TEXT("actor : %s , myteamide : %d"), *GetActorNameOrLabel(), currentteam.GetId());

	
	
}

//인풋 매핑 - 액션에 함수 바인딩
void APGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	// 인풋 매핑이 있다면
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		/*EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);*/

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

		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &APGPlayerCharacter::OnDash);

		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Started, this, &APGPlayerCharacter::InputSkill);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APGPlayerCharacter::StopDefenceNikke);

		EnhancedInputComponent->BindAction(UltiSkillAction, ETriggerEvent::Started, this, &APGPlayerCharacter::OnUltimateSkill);
	
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
	if (!bIsNikkeSkill)
		SpringArm->TargetArmLength = NewCharacterData->TargetArmLength;
	else
		SpringArm->TargetArmLength = 300.0f;

	SpringArm->SetRelativeRotation(NewCharacterData->RelativeRotation);
	SpringArm->bUsePawnControlRotation = NewCharacterData->bUsePawnControlRotation;
	SpringArm->bInheritPitch = NewCharacterData->bInheritPitch;
	SpringArm->bInheritYaw = NewCharacterData->bInheritYaw;
	SpringArm->bInheritRoll = NewCharacterData->bInheritRoll;
	SpringArm->bDoCollisionTest = NewCharacterData->bDoCollisionTest;
	if (!bIsNikkeSkill)
		SpringArm->SetRelativeTransform(NewCharacterData->transform);
	else
	{
		FTransform nikkeTargetArm = NewCharacterData->transform;
		FVector currentLocation = nikkeTargetArm.GetLocation();
		currentLocation.Z = -10.0f;
		nikkeTargetArm.SetLocation(currentLocation);
		SpringArm->SetRelativeTransform(nikkeTargetArm);
	}
		

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
	if (bIsSlow||bIsDash)return;

	if (bIsAim)
	{
		bIsShoot = true;
	}
	else
	{
		if (bIsNikkeSkill)
			StopDefenceNikke();
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
	if (bIsSlow || bIsDash|| bIsUltiSkill)return;

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

		if (bIsNikkeSkill)
		{
			FRotator BackRotation = GetActorRotation() + FRotator(0, 120.0f, 0);
			SetActorRotation(BackRotation);
		}
	

}

void APGPlayerCharacter::PressReload()
{
	if (bIsSlow||bIsDash||bIsUltiSkill) return;
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
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//대쉬 중
	if (bIsDash)
	{
		if (bIsEvade) return DamageAmount;

		OnAvoidEffect();
		SetEvadeRotation(DamageCauser->GetActorLocation());
	}
	//무적
	if (AttackComponent->GetbIsGodMode()) return DamageAmount;
	
	//적팀 한테 데미지
	if (GetTeamAttitudeTowards(*DamageCauser) && !DamageCauser->ActorHasTag(TAG_GRENADE))
	{
		
		StatComponent->Damaged(DamageAmount);
	}

	//수류탄에 맞았을시
	if (DamageCauser->ActorHasTag(TAG_GRENADE) )
	{

		FVector Direction = GetActorLocation() - DamageCauser->GetActorLocation();
		Direction.Normalize();
		HitImpulseVector = Direction * 750.0f + (FVector(0, 0, 1) * 100.0f);

		if (GetTeamAttitudeTowards(*EventInstigator->GetPawn()))
		{
			StatComponent->Damaged(DamageAmount);
		}
		else
		{
			StatComponent->Damaged(DamageAmount * 0.3f);
		}
	}
	
	
	return DamageAmount;
}


void APGPlayerCharacter::SetUpHudWidget(UPGHudWidget* hudWidget)
{
	if (hudWidget)
	{
		hudWidget->SetUpWidget(StatComponent->GetBaseStat(), StatComponent->GetModifierStat());
		hudWidget->SetupUltiSkillWidget(StatComponent->GetCurrentUltiSkillGauge());
		hudWidget->SetupSkillWidget(AttackComponent->GetSkill()->GetSkillCooltime());
		hudWidget->SetupDashWidget(DashColltime);
		hudWidget->UpdateHpBar(StatComponent->GetCurrentHp());
		hudWidget->UpdateHitGaugeBar(StatComponent->GetCurrentHitGauge());
	
		//델리게이트 바인딩
		StatComponent->OnStatChanged.AddUObject(hudWidget, &UPGHudWidget::SetUpWidget);
		StatComponent->OnHpChanged.AddUObject(hudWidget, &UPGHudWidget::UpdateHpBar);
		StatComponent->OnHitGaugeChanged.AddUObject(hudWidget, &UPGHudWidget::UpdateHitGaugeBar);
		StatComponent->OnUltiSkillGaugechanged.AddUObject(hudWidget, &UPGHudWidget::UpdateUltiSkillGaugeBar);
		AttackComponent->GetSkill()->OnbCanSkill.AddUObject(hudWidget, &UPGHudWidget::StartSkillCoolTime);
		OndashDelegate.AddUObject(hudWidget, &UPGHudWidget::StartDash);

		//총이 있을때만
		ARifle* rifle = Cast<ARifle>(AttackComponent->GetWeapon());
		if (rifle)
		{
			hudWidget->SetupGunWidget(rifle->GetammoMaxCount());
			rifle->OnAmmoChanged.AddUObject(hudWidget, &UPGHudWidget::UpdateGunAmmo);

			OnbIsAim.AddUObject(hudWidget, &UPGHudWidget::SetGunWidgetEnalbe);
		}
		
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
			ResetMotionWarping();
			return;
		}

	}
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(TEXT("Target"), targetPos);
	
}

void APGPlayerCharacter::ResetMotionWarping()
{
	MotionWarpingComponent->RemoveWarpTarget(TEXT("Target"));
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
	
	Camera->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = true;
	Camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	Camera->PostProcessSettings.DepthOfFieldFocalDistance = 300.0f;
	Camera->PostProcessSettings.DepthOfFieldSensorWidth = 750.0f;

	Camera->PostProcessSettings.bOverride_SceneFringeIntensity = true;
	Camera->PostProcessSettings.SceneFringeIntensity = 2.5f;


	}
	else
	{
		
		Camera->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = false;
		Camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = false;

		Camera->PostProcessSettings.bOverride_SceneFringeIntensity = false; 
	}
}

void APGPlayerCharacter::OnDash()
{
	if(DashCooltimeTimerHandle.IsValid()) return;
	if (bIsUltiSkill)return;

	OndashDelegate.Broadcast();
	bIsDash = true;
	AttackComponent->SetbIsGodMode(true);
	 OriginalMaxWalkSpeed = GetCharacterMovement()->GetMaxSpeed();
	 OriginalMaxAcceleration = GetCharacterMovement()->GetMaxAcceleration();

	GetCharacterMovement()->MaxWalkSpeed = OriginalMaxWalkSpeed*2.0f;
	GetCharacterMovement()->MaxAcceleration = OriginalMaxAcceleration*3.0f;

	DashVector = GetCharacterMovement()->GetLastInputVector();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(DashMontage, 1.0f);
	
	//GetController()->SetIgnoreMoveInput(true);
	
	GetWorld()->GetTimerManager().SetTimer(
		DashTimerHandle,
		[this]() {
			
			GetCharacterMovement()->MaxWalkSpeed = OriginalMaxWalkSpeed;
			GetCharacterMovement()->MaxAcceleration = OriginalMaxAcceleration;
			//GetController()->SetIgnoreMoveInput(false);
			GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle); 

			bIsDash = false;
			AttackComponent->SetbIsGodMode(false);

		}, DashTime, false
	);

	//대쉬 쿨타임
	GetWorld()->GetTimerManager().SetTimer(
		DashCooltimeTimerHandle,
		[this]() {

			GetWorld()->GetTimerManager().ClearTimer(DashCooltimeTimerHandle);

		}, DashColltime, false
	);
}

void APGPlayerCharacter::OnAvoidEffect()
{

	//SLOG(TEXT("Avoid"));
	bIsEvade = true;

	PlayEvadeMontage();
	PlayEvadeCameraShake();
	OnEvadePostPorcess(true);

	CustomTimeDilation = 0.3f;

	GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
	GetCharacterMovement()->MaxWalkSpeed = OriginalMaxWalkSpeed;
	GetCharacterMovement()->MaxAcceleration = OriginalMaxAcceleration;


	OnSlowOVerlapToNPC(EvadeTime);

	GetWorld()->GetTimerManager().SetTimer(
		EvadeTimerHandle,
		[this]() {

			OnEvadePostPorcess(false);
			CustomTimeDilation = 1.3f;
			GetWorld()->GetTimerManager().ClearTimer(EvadeTimerHandle);

		}, EvadeTime, false
	);

}

void APGPlayerCharacter::PlayEvadeMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	// 오른쪽방향
	if (DashVector.X > 0)
	{
		CurrentEvadeMontage = RightEvadeMontage;
	}
	else
	{
		CurrentEvadeMontage = LeftEvadeMontage;
	}

	AnimInstance->Montage_Play(CurrentEvadeMontage, 1.0f);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APGPlayerCharacter::EndEvadeMontage);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, CurrentEvadeMontage);
}

void APGPlayerCharacter::PlayEvadeCameraShake()
{
	IPlayerCameraShakeInterface* playerCamera = Cast<IPlayerCameraShakeInterface>(GetController());
	if (playerCamera)
	{
		playerCamera->PlayCameraShake(EvadeCameraShakeClass);
	}

}

void APGPlayerCharacter::OnEvadePostPorcess(bool effect)
{
	if (effect)
	{
		

		Camera->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = true;
		Camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
		Camera->PostProcessSettings.DepthOfFieldFocalDistance = 300.0f;
		Camera->PostProcessSettings.DepthOfFieldSensorWidth = 750.0f;


	}
	else
	{
		Camera->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = false;
		Camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = false;

	}
}

void APGPlayerCharacter::EndEvadeMontage(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	GetWorld()->GetTimerManager().ClearTimer(EvadeTimerHandle);
	bIsDash = false;
	bIsEvade = false;
	AttackComponent->SetbIsGodMode(false);
}

void APGPlayerCharacter::SetEvadeRotation(FVector TargetVector)
{
	FVector CharacterLocation = GetActorLocation();

	// 타겟 방향 벡터를 계산합니다.
	FVector DirectionToTarget = (TargetVector - CharacterLocation).GetSafeNormal();

	// 타겟 방향으로 회전합니다.
	FRotator NewRotation = DirectionToTarget.Rotation();

	SetActorRotation(NewRotation);
}

void APGPlayerCharacter::OnSlowOVerlapToNPC(float time)
{
	FVector Center = GetActorLocation();

	//float SlowRadius = 500.0f;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(SLowMotion), false, this);

	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_PGACTION,
		FCollisionShape::MakeSphere(SlowRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{

			INPCParryCheckInterface* NPC = Cast<INPCParryCheckInterface>(OverlapResult.GetActor());
			if (NPC)
			{
				NPC->NPCAttackHitStop(time);
				DrawDebugSphere(GetWorld(), Center, SlowRadius, 16, FColor::Purple, false, 0.3f);

			}
		}
	}
}

bool APGPlayerCharacter::CanPlayerTarget(APawn* enemy)
{
	if (TargetMePawns.Num()>= MaxTargets)
	{
		if (TargetMePawns.Contains(enemy))
			return true;
		else
			return false;
	}
	else
		return true;

	
	
}

void APGPlayerCharacter::SetPlayerTargetPawn(APawn* enemy)
{
	if (!TargetMePawns.Contains(enemy))
	{
		TargetMePawns.Add(enemy);
		
	}


}

void APGPlayerCharacter::DeletePlayerTargetPawn(APawn* enemy)
{
	if (TargetMePawns.Contains(enemy))
	{
		TargetMePawns.Remove(enemy);
		
	}
}

void APGPlayerCharacter::InputSkill()
{
	if (bIsSlow || bIsDash)return;

	SkillToComponent();
	
}

void APGPlayerCharacter::SetbIsNikkeSkill(bool skill)
{
	Super::SetbIsNikkeSkill(skill);

	if (skill)
	{

		SetCharacterData(EControlData::Base);
	}
	else
	{

		SetCharacterData(EControlData::Base);
	}

}

void APGPlayerCharacter::StopDefenceNikke()
{
	SetbIsNikkeSkill(false);
}

void APGPlayerCharacter::OnUltimateSkill()
{
	UltimateSkillToComponent();
}

ULevelSequence* APGPlayerCharacter::GetLevelSequence()
{
	return LevelSequence;
}

void APGPlayerCharacter::ChangeViewTarget(bool bstart)
{
	
	APGPlayerController* playerController = Cast<APGPlayerController>(GetController());
	if (playerController)
	{
		if (bstart)
		{
			playerController->SetViewTargetWithBlend(this, 0.5f); 
			Camera->SetActive(false);
			CutSceneCamera->SetActive(true);
		}
		else
		{
			playerController->SetViewTargetWithBlend(this, 0.5f);
			Camera->SetActive(true);
			CutSceneCamera->SetActive(false);
		}
	}
	
}













