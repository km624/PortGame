// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PGCharacterData.h"
#include "Components/TimelineComponent.h"
#include "UI/PGHudWidget.h"
#include "Component/PGStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "PortGame/PortGame.h"
#include "MotionWarpingComponent.h"
#include "Component/TargetingComponent.h"
#include "Component/PGAttackComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Physics/PGCollision.h"
#include "Engine/OverlapResult.h"
#include "Interface/NPCParryCheckInterface.h"
#include "Interface/PlayerCameraShakeInterface.h"

#include "Skill/SkillBase.h"
#include "Player/PGPlayerController.h"
#include "Weapon/Rifle.h"
#include "Data/PlayerCharacterDataAsset.h"

#include "UI/PGCharcterWidget.h"
#include "UI/PGAllCharactersWidget.h"
#include "UI/PGCharcterWidget.h"

#include "Component/AIBodyGuardComponent.h"

#include "Data/BGBaseOptionDataAsset.h"

#include "LevelSequenceActor.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"

#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Data/CharacterVoiceDataAsset.h"


const FString APGPlayerCharacter::LeftEvadeMontage = TEXT("LeftEvadeMontage");
const FString APGPlayerCharacter::RightEvadeMontage = TEXT("RightEvadeMontage");
const FString APGPlayerCharacter::DashMontage = TEXT("DashMontage");
const FString APGPlayerCharacter::ExcuisonAttackMontage = TEXT("ExcuisonAttackMontage");

APGPlayerCharacter::APGPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	TargetingComponent = CreateDefaultSubobject<UTargetingComponent>(TEXT("Targeting_Comp"));

	static ConstructorHelpers::FObjectFinder<UPGCharacterData> CharacterBaseData
	(TEXT("/Script/PortGame.PGCharacterData'/Game/PortGame/Input/PG_InputBaseData.PG_InputBaseData'"));
	if (CharacterBaseData.Object)
		ControlDataManager.Add(EControlData::Base, CharacterBaseData.Object);

	static ConstructorHelpers::FObjectFinder<UPGCharacterData> CharacterAimData
	(TEXT("/Script/PortGame.PGCharacterData'/Game/PortGame/Input/PG_InputAimData.PG_InputAimData'"));
	if (CharacterAimData.Object)
		ControlDataManager.Add(EControlData::Aim, CharacterAimData.Object);
	static ConstructorHelpers::FObjectFinder<UCurveFloat> aimCurve(TEXT("/Script/Engine.CurveFloat'/Game/PortGame/Weapon/AimCurve.AimCurve'"));
	if (aimCurve.Object)
	{
		AimCurve = aimCurve.Object;
	}

	//Hud위젯
	static ConstructorHelpers::FClassFinder<UPGHudWidget> ABHUDWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PortGame/UI/BP_HudWidget.BP_HudWidget_C'"));
	if (ABHUDWidgetRef.Class)
	{
		PGHudWidgetClass = ABHUDWidgetRef.Class;
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

	static ConstructorHelpers::FObjectFinder<UInputAction> OneC(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_OneChangeCharacter.IA_OneChangeCharacter'"));
	if (OneC.Object)
	{
		OneChangeCharacterAction = OneC.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> TwoC(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_TwoChangeCharacter.IA_TwoChangeCharacter'"));
	if (TwoC.Object)
	{
		TwoChangeCharacterAction = TwoC.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ThreeC(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_ThreeChangeCharacter.IA_ThreeChangeCharacter'"));
	if (ThreeC.Object)
	{
		ThreeChangeCharacterAction = ThreeC.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MAPC(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Map.IA_Map'"));
	if (MAPC.Object)
	{
		MapAction = MAPC.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> BODYGUARD(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_BodyguardOption.IA_BodyguardOption'"));
	if (BODYGUARD.Object)
	{
		BodyGuardOptionAction = BODYGUARD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> EXECUTION(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Excuison.IA_Excuison'"));
	if (EXECUTION.Object)
	{
		ExecutionAction = EXECUTION.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CCurve(TEXT("/Script/Engine.CurveFloat'/Game/PortGame/Weapon/AttackCameraCurve.AttackCameraCurve'"));
	if (CCurve.Object)
	{
		AttackCurve = CCurve.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> DCurve(TEXT("/Script/Engine.CurveFloat'/Game/PortGame/Weapon/DashCurve.DashCurve'"));
	if (DCurve.Object)
	{
		DashCurve = DCurve.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> BodyCurve(TEXT("/Script/Engine.CurveFloat'/Game/PortGame/Weapon/BodyGuardCurve.BodyGuardCurve'"));
	if (BodyCurve.Object)
	{
		BodyGuardCurve = BodyCurve.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> FieldCurve(TEXT("/Script/Engine.CurveFloat'/Game/PortGame/Weapon/FieldChangeCurve.FieldChangeCurve'"));
	if (FieldCurve.Object)
	{
		FieldChangeCurve = FieldCurve.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> AMCurve(TEXT("/Script/Engine.CurveFloat'/Game/PortGame/Weapon/ArmorBreakCamera.ArmorBreakCamera'"));
	if (AMCurve.Object)
	{
		ArmorBreakCurve = AMCurve.Object;
	}
	static ConstructorHelpers::FObjectFinder<UCurveFloat> EBCurve(TEXT("/Script/Engine.CurveFloat'/Game/PortGame/Weapon/EyeBllinkCurve.EyeBllinkCurve'"));
	if (EBCurve.Object)
	{
		EyeBlinkCurve = EBCurve.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>BaseMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/PortGame/Effect/Material/M_Scan_Inst.M_Scan_Inst'"));
	if (BaseMaterial.Object)
	{
		PostProcessMaterial = BaseMaterial.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>evadesound(TEXT("/Script/Engine.SoundWave'/Game/PortGame/Sound/SFX/EvadeSFX.EvadeSFX'"));
	if (evadesound.Object)
	{
		EvadeSFX = evadesound.Object;
	}


	ExecutionLevelSequenceClass = ALevelSequenceActor::StaticClass();

	AIBodyGuardComponent = CreateDefaultSubobject<UAIBodyGuardComponent>(TEXT("AIBodyComponent"));

	VoiceComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	VoiceComponent->bAutoActivate = false;
	VoiceComponent->RegisterComponent();
	VoiceComponent->bAllowSpatialization = true; // 3D 사운드 활성화
	

	Tags.Add(TAG_PLAYER);

	bIsGameStated = false;

	bShowBodyGuardOption = false;

	bIsExecutionRange = false;

	GetMesh()->SetCustomDepthStencilValue(1);
}

void APGPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(baseCharacterData)
		CreateHudWidget();

	bIsGlobalTimeSlow = false;
}

void APGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//SetCharacterInputData(CurrentControlData);

	StatComponent->OnLevelChanged.AddUObject(this, &ThisClass::PlayLevelUpEffet);

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ThisClass::OnComponentHit);

	FGenericTeamId currentteam = GetGenericTeamId();

	AllTimelineSetting();

	SetPostProcessMaterial();

	SetEyeBlinkTimeline();
}

void APGPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DashCooltimeTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(EvadeTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(AIBodyGuardComponent->BGGuageTimer);
}

//인풋 매핑 - 액션에 함수 바인딩
void APGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	// 인풋 매핑이 있다면
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APGPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APGPlayerCharacter::SetNoneMove);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APGPlayerCharacter::Look);

		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APGPlayerCharacter::Attack);
		if (CharacterType == EPlayerCharacterType::BlueArchive || CharacterType == EPlayerCharacterType::Nikke)
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

		EnhancedInputComponent->BindAction(OneChangeCharacterAction, ETriggerEvent::Started, this, &APGPlayerCharacter::OneChangePlayerCharacter);
		EnhancedInputComponent->BindAction(TwoChangeCharacterAction, ETriggerEvent::Started, this, &APGPlayerCharacter::TwoChangePlayerCharacter);
		EnhancedInputComponent->BindAction(ThreeChangeCharacterAction, ETriggerEvent::Started, this, &APGPlayerCharacter::ThreeChangePlayerCharacter);

		EnhancedInputComponent->BindAction(MapAction, ETriggerEvent::Started, this, &APGPlayerCharacter::ChangeMiniMapSize);
		
		
		EnhancedInputComponent->BindAction(BodyGuardOptionAction, ETriggerEvent::Started, this, &APGPlayerCharacter::ShowBodyGuardOption);
		EnhancedInputComponent->BindAction(BodyGuardOptionAction, ETriggerEvent::Completed, this, &APGPlayerCharacter::CloseBodyGuardOption);
		
		EnhancedInputComponent->BindAction(ExecutionAction, ETriggerEvent::Started, this, &APGPlayerCharacter::OnExecution);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void APGPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	AllTimelineTick(DeltaTime);

	if (TargetingComponent->GetbIsTargetLock())
	{
		TargetingComponent->TargetLockOn(DeltaTime);
	}
	else
	{
		if (bIsAttackRotation && bIsMoving)
			SetAttackRotation(DeltaTime);

	}
	
	EyeBlinkTimeline.TickTimeline(DeltaTime);

}

void APGPlayerCharacter::SetupCharacterData(UBaseCharacterDataAsset* characterdata)
{

	UPlayerCharacterDataAsset* palyerdata = Cast<UPlayerCharacterDataAsset>(characterdata);

	LevelSequence = palyerdata->LevelSequence;
	ExecutionLevelSequence = palyerdata->ExecutionLevelSequence;
	VoiceDataAsset = palyerdata->VoiceDataAsset;
	Super::SetupCharacterData(characterdata);

	StatComponent->SetCurrentRarity(palyerdata->Rarity);
	
	LoadAndPlayMontageByPath(CharacterName, DashMontage);
	LoadAndPlayMontageByPath(CharacterName, LeftEvadeMontage);
	LoadAndPlayMontageByPath(CharacterName, RightEvadeMontage);
	LoadAndPlayMontageByPath(CharacterName, ExcuisonAttackMontage);
}

void APGPlayerCharacter::SetUpBodyGuardOption(TArray<UBGBaseOptionDataAsset*>& optionDataAssets)
{
	if (optionDataAssets.Num() != 0)
	{
		OptionDataAssets = optionDataAssets;
		AIBodyGuardComponent->SetUpBodyGuardOptions(OptionDataAssets);
	}
	else
	{
		SLOG(TEXT("NoOptionData : playercharacter"));
	}
	
}

void APGPlayerCharacter::SetCharacterInputData(EControlData DataName)
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
	if (bIsGameStated)
		return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{

		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APGPlayerCharacter::Attack()
{
	if (bIsSlow || bIsDash || !bIsMiniMap ||bIsGameStated|| bShowBodyGuardOption)return;

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
		bIsShoot = true;
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
	if (bIsSlow || bIsDash || bIsUltiSkill || !bIsMiniMap || bIsGameStated|| bShowBodyGuardOption)return;

	bIsAim = true;
	OnbIsAim.Broadcast(bIsAim);

	SetCharacterInputData(EControlData::Aim);
	//AimTimeline.Play();
	StartSetCameraMoveSetting(false, ECameraMoveType::AimCamera);

	AimLocation = Camera->GetComponentLocation();


}

void APGPlayerCharacter::OnGoingAim()
{
	bIsAim = true;
	OnbIsAim.Broadcast(bIsAim);
	AimLocation = Camera->GetComponentLocation();
}

void APGPlayerCharacter::ReleasedAim()
{

	bIsAim = false;
	OnbIsAim.Broadcast(bIsAim);

	bIsShoot = false;
	OnbIsShoot.Broadcast(bIsShoot);
	SetCharacterInputData(EControlData::Base);
	if(!bShowBodyGuardOption || !bIsGlobalTimeSlow)
		StartSetCameraMoveSetting(true, ECameraMoveType::AimCamera);
	//AimTimeline.Reverse();

	if (bIsNikkeSkill)
	{
		FRotator BackRotation = GetActorRotation() + FRotator(0, 120.0f, 0);
		SetActorRotation(BackRotation);
	}


}

void APGPlayerCharacter::PressReload()
{
	if (bIsSlow || bIsDash || bIsUltiSkill || !bIsMiniMap|| bShowBodyGuardOption) return;
	ReloadToWeapon();
}



void APGPlayerCharacter::AimUpdate(float deltaTime)
{

	float AimX;
	float AimY;
	float AimZ;

	if (!bIsReversed)
	{
		AimX = FMath::Lerp(CameraCurrentLocation.X, 150.0f, deltaTime);
		AimY = FMath::Lerp(CameraCurrentLocation.Y, 75.0f, deltaTime);
		
	}
	else
	{
		AimX = FMath::Lerp(CameraCurrentLocation.X, 0.0f, deltaTime);
		AimY = FMath::Lerp(CameraCurrentLocation.Y, 0.0f, deltaTime);
	
	}
	AimZ = FMath::Lerp(CameraCurrentLocation.Z, 0.0f, deltaTime);

	Camera->SetRelativeLocation(FVector(AimX, AimY, AimZ));
	Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	if (DynamicMaterialInstance)
		DynamicMaterialInstance->SetScalarParameterValue(FName("Alpha"), 0.0f);

}

float APGPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!EventInstigator || !EventInstigator->GetPawn()) return DamageAmount;

	APGBaseCharacter* attackPawn = Cast<APGBaseCharacter>(EventInstigator->GetPawn());
	if (attackPawn)
	{
		// 대쉬 중
		if (bIsDash)
		{
			if (bIsEvade) return DamageAmount;

			if (DamageCauser)
			{
				OnAvoidEffect();
				SetEvadeRotation(DamageCauser->GetActorLocation());
			}
		}
		// 무적
		if (AttackComponent && AttackComponent->GetbIsGodMode()) return DamageAmount;

		// 적팀한테 데미지
		if (DamageCauser && GetTeamAttitudeTowards(*DamageCauser) && !DamageCauser->ActorHasTag(TAG_GRENADE))
		{
			if (StatComponent)
			{
				StatComponent->Damaged(DamageAmount, DamageCauser);
			}
		}

	}

	return DamageAmount;
}

void APGPlayerCharacter::PlayHitMontage()
{
	Super::PlayHitMontage();

	PlayCharacterVoice(ECharacterVoiceType::Damage);
}


void APGPlayerCharacter::SetUpHudWidget(UPGHudWidget* hudWidget)
{
	//SLOG(TEXT("%s : Setup"), *GetActorNameOrLabel());
	if (hudWidget)
	{
		hudWidget->SetUpWidget(StatComponent->GetBaseStat(), StatComponent->GetModifierStat(), StatComponent->GetLevelStat());
		hudWidget->SetupUltiSkillWidget(StatComponent->GetCurrentUltiSkillGauge());
		hudWidget->SetupSkillWidget(AttackComponent->GetSkill()->GetSkillCooltime());
		hudWidget->SetupDashWidget(DashColltime);
		hudWidget->UpdateHpBar(StatComponent->GetCurrentHp());
		hudWidget->UpdateHitGaugeBar(StatComponent->GetCurrentHitGauge());
		hudWidget->UpdateKOCount(KOCount);
		hudWidget->SetUpProtectMaxCount(AIBodyGuardComponent->GetMaxProtectCount());
		hudWidget->SetupBodyGuardOptionButton(this, OptionDataAssets);
		hudWidget->SetUpAllBGGauge(AIBodyGuardComponent->GetMaxBGGaugeCount(), AIBodyGuardComponent->GetCurrentBGGauge());

		//델리게이트 바인딩
		StatComponent->OnStatChanged.AddUObject(hudWidget, &UPGHudWidget::SetUpWidget);
		StatComponent->OnHpChanged.AddUObject(hudWidget, &UPGHudWidget::UpdateHpBar);
		StatComponent->OnHitGaugeChanged.AddUObject(hudWidget, &UPGHudWidget::UpdateHitGaugeBar);
		StatComponent->OnUltiSkillGaugechanged.AddUObject(hudWidget, &UPGHudWidget::UpdateUltiSkillGaugeBar);
		AttackComponent->GetSkill()->OnbCanSkill.AddUObject(hudWidget, &UPGHudWidget::StartSkillCoolTime);
		OndashDelegate.AddUObject(hudWidget, &UPGHudWidget::StartDash);
		FKoCountChanged.AddUObject(hudWidget, &UPGHudWidget::UpdateKOCount);
		AIBodyGuardComponent->OnProtectCountChanged.AddUObject(hudWidget, &UPGHudWidget::UpdateProtectCount);
		AIBodyGuardComponent->BGGaugeChanaged.AddUObject(hudWidget, &UPGHudWidget::UpdateBGGauge);

		//총이 있을때만
		ARifle* rifle = Cast<ARifle>(AttackComponent->GetWeapon());
		if (rifle)
		{
			hudWidget->SetupGunWidget(rifle->GetammoMaxCount());
			rifle->OnAmmoChanged.AddUObject(hudWidget, &UPGHudWidget::UpdateGunAmmo);

			//shudWidget->SetupReloadWidget(rifle->GetReloadMotagetime());
			rifle->OnbIsGunReload.AddUObject(hudWidget, &UPGHudWidget::StartReload);

			rifle->OnbInGunRanged.AddUObject(hudWidget, &UPGHudWidget::ChangeCrosshair);

			OnbIsAim.AddUObject(hudWidget, &UPGHudWidget::SetGunWidgetEnable);
		}
		//SLOG(TEXT("SetupendWidget : %s"), *GetActorNameOrLabel());
	}
}

void APGPlayerCharacter::FindClosestEnemyToComp()
{
	TargetingComponent->SetTargetLock();
}

void APGPlayerCharacter::FindSideEnemyToComp(const FInputActionValue& Value)
{

	float direction = Value.Get<float>();
	if (TargetingComponent->GetbIsTargetLock())
	{
		TargetingComponent->SetSideTargetLock(direction*-1.0f);
	}
	else
	{
		float addFov = Camera->FieldOfView += -direction * 2.5f ;
		float newFov = FMath::Clamp(addFov, 50.0f, 110.0f);
		Camera->SetFieldOfView(newFov);
	}
		


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
		if (bIsExecution)
		{
			
			CutSceneCamera->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = true;
			CutSceneCamera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
			CutSceneCamera->PostProcessSettings.DepthOfFieldFocalDistance = 500.0f;
			CutSceneCamera->PostProcessSettings.DepthOfFieldSensorWidth = 2000.0f;;

			CutSceneCamera->PostProcessSettings.bOverride_SceneFringeIntensity = true;
			CutSceneCamera->PostProcessSettings.SceneFringeIntensity = 2.5f;
		}
		else
		{
			
			Camera->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = true;
			Camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
			Camera->PostProcessSettings.DepthOfFieldFocalDistance = 500.0f;
			Camera->PostProcessSettings.DepthOfFieldSensorWidth = 2000.0f;;

			Camera->PostProcessSettings.bOverride_SceneFringeIntensity = true;
			Camera->PostProcessSettings.SceneFringeIntensity = 2.5f;
		}
		

	}
	else
	{
		if (bIsExecution)
		{
			CutSceneCamera->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = false;
			CutSceneCamera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = false;

			CutSceneCamera->PostProcessSettings.bOverride_SceneFringeIntensity = false;
		}
		else
		{
			Camera->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = false;
			Camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = false;

			Camera->PostProcessSettings.bOverride_SceneFringeIntensity = false;
		}

		
	}
}

void APGPlayerCharacter::OnDash()
{
	if (DashCooltimeTimerHandle.IsValid()) return;
	if (bIsUltiSkill)return;

	OndashDelegate.Broadcast();
	bIsDash = true;
	AttackComponent->SetbIsGodMode(true);
	OriginalMaxWalkSpeed = GetCharacterMovement()->GetMaxSpeed();
	OriginalMaxAcceleration = GetCharacterMovement()->GetMaxAcceleration();
	
	GetCharacterMovement()->MaxWalkSpeed = OriginalMaxWalkSpeed * 2.0f;
	GetCharacterMovement()->MaxAcceleration = OriginalMaxAcceleration * 3.0f;

	DashVector = GetCharacterMovement()->GetLastInputVector();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AllMontage[DashMontage], 1.0f);

	//GetController()->SetIgnoreMoveInput(true);

	CustomTimeDilation = 1.0f;

	StartSetCameraMoveSetting(false, ECameraMoveType::DashCamera);

	GetWorld()->GetTimerManager().SetTimer(
		DashTimerHandle,
		[this]() {

			GetCharacterMovement()->MaxWalkSpeed = OriginalMaxWalkSpeed;
			GetCharacterMovement()->MaxAcceleration = OriginalMaxAcceleration;
			
			GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
			if(!bShowBodyGuardOption||!bIsGlobalTimeSlow)
				StartSetCameraMoveSetting(true, ECameraMoveType::DashCamera);

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


	OnSlowOVerlapToNPC(EvadeTime,NULL);

	if (EvadeSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EvadeSFX, GetOwner()->GetActorLocation());
	}

	GetWorld()->GetTimerManager().SetTimer(
		EvadeTimerHandle,
		[this]() {

			OnEvadePostPorcess(false);
			CustomTimeDilation = 1.0f;
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
		CurrentEvadeMontage = AllMontage[RightEvadeMontage];
	}
	else
	{
		CurrentEvadeMontage = AllMontage[LeftEvadeMontage];
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
		Camera->PostProcessSettings.DepthOfFieldFocalDistance = 500.0f;
		Camera->PostProcessSettings.DepthOfFieldSensorWidth = 2000.0f;


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

void APGPlayerCharacter::OnSlowOVerlapToNPC(float time , AActor* ignoreActor)
{
	
	FVector Center = GetActorLocation();

	//float SlowRadius = 500.0f;
	TArray<FOverlapResult> OverlapResults;
	
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(SLowMotion), false, this);

	if (ignoreActor != nullptr)
	{
		CollisionQueryParam.AddIgnoredActor(ignoreActor);
		
	}
	else
	{
		CollisionQueryParam.ClearIgnoredActors();
	}

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

void APGPlayerCharacter::InputSkill()
{
	if (bIsSlow || bIsDash|| bIsGameStated)return;

	SkillToComponent();

}

void APGPlayerCharacter::SetbIsNikkeSkill(bool skill)
{
	Super::SetbIsNikkeSkill(skill);

	if (skill)
	{

		SetCharacterInputData(EControlData::Base);
	}
	else
	{

		SetCharacterInputData(EControlData::Base);
	}

}

void APGPlayerCharacter::StopDefenceNikke()
{
	SetbIsNikkeSkill(false);
}

void APGPlayerCharacter::OnUltimateSkill()
{
	if (bIsGameStated|| bShowBodyGuardOption)return;
	
	UltimateSkillToComponent();
}

ULevelSequence* APGPlayerCharacter::GetLevelSequence()
{
	return LevelSequence;
}

void APGPlayerCharacter::ChangeViewTarget(bool bstart)
{

	if (bstart)
	{

		Camera->SetActive(false);
		CutSceneCamera->SetActive(true);
	}
	else
	{
		GetController()->SetIgnoreLookInput(false);
		Camera->SetActive(true);
		CutSceneCamera->SetActive(false);
	}
}



void APGPlayerCharacter::CreateHudWidget()
{
	
	PGHudWidget = CreateWidget<UPGHudWidget>(GetWorld(), PGHudWidgetClass);
	if (PGHudWidget)
	{

		PGHudWidget->SetOwingCharcter(this);
		SetUpHudWidget(PGHudWidget);
		

	}
}

void APGPlayerCharacter::HudWidgetAddviewport()
{
	if (PGHudWidget)
		PGHudWidget->AddToViewport();

	
}

void APGPlayerCharacter::RemoveHudWidget()
{
	if (PGHudWidget)
		PGHudWidget->RemoveFromParent();
	
}

void APGPlayerCharacter::OneChangePlayerCharacter()
{
	int8 num = 0;
	CheckandChangePlayerCharacter(num);
}

void APGPlayerCharacter::TwoChangePlayerCharacter()
{
	int8 num = 1;
	CheckandChangePlayerCharacter(num);
}

void APGPlayerCharacter::ThreeChangePlayerCharacter()
{
	int8 num = 2;
	CheckandChangePlayerCharacter(num);
}

void APGPlayerCharacter::CheckandChangePlayerCharacter(int8 num)
{
	if (bIsGlobalTimeSlow)
		return;

	if (TargetingComponent->GetbIsTargetLock())
	{
		TargetingComponent->ResetTargeting();
	}

	APGPlayerController* playerController = Cast<APGPlayerController>(GetController());
	if (playerController)
		playerController->ChangedCharacterPossess(num);
}

void APGPlayerCharacter::SetupAllCharacterWidget(int32 num)
{
	if (PGHudWidget)
		PGHudWidget->SetUpAllCharactersWidget(num);
}

void APGPlayerCharacter::SetupMyCharacterWidgetToAnother(const TArray<APGPlayerCharacter*>& allcharacters,float cooltime)
{
	if (allcharacters.Num() > 0)
	{

		if (allcharacters.Contains(this))
		{
			int32 num = allcharacters.IndexOfByKey(this);
			bool bisMine = false;
			for (int32 i=0;i< allcharacters.Num();i++)
			{
				if (i == num)
					bisMine = true;
				else
					bisMine = false;

				//캐릭터별 ui와 셋업, 바인딩 하기위한 똥꼬쇼
				UPGHudWidget* hud = allcharacters[i]->GetPlayerHudWidget();
				if (hud)
				{
					UPGAllCharactersWidget* allwidget =  hud->GetAllCharacterWidgets();
					if (allwidget)
					{
						allwidget->SetupCharacterWdiget(num, CharacterType, 
							StatComponent->GetBaseStat(), StatComponent->GetModifierStat(), StatComponent->GetLevelStat(), bisMine, cooltime);


						TArray<UPGCharcterWidget*> const characterwidgets = allwidget->GetPCharacterWidgets();

						if (characterwidgets.IsValidIndex(num))
						{

							StatComponent->OnStatChanged.AddUObject(characterwidgets[num], &UPGCharcterWidget::SetUphpbarWidget);
							StatComponent->OnHpChanged.AddUObject(characterwidgets[num], &UPGCharcterWidget::UpdateHpBar);
							StatComponent->OnHitGaugeChanged.AddUObject(characterwidgets[num], &UPGCharcterWidget::UpdateHitGaugeBar);
							StatComponent->OnHpZero.AddUObject(characterwidgets[num], &UPGCharcterWidget::SetDead);
						}

					}
				}
			}
		}
	
	}
}

void APGPlayerCharacter::OnStartChangeCharacterWidget(int32 num)
{
	if (PGHudWidget)
	{
		PGHudWidget->OnStartChangeCooltime(num);
		
	}

		
}

void APGPlayerCharacter::SetupPlayerMiniMap(const TArray<class APGPlayerCharacter*>& allcharacters, const TArray<APGAIController*>& allaicontrollers)
{
	if (PGHudWidget)
	{
		TArray<AActor*> ActorArray;
		int8 num = 0;
		if (allcharacters.Num() > 0)
		{

			if (allcharacters.Contains(this))
			{
				num = allcharacters.IndexOfByKey(this);

			}
		}

		//액터로 변환
		for (APGPlayerCharacter* Character : allcharacters)
		{
			ActorArray.Add(Character); 
		}

		if (ActorArray.Num() > 0)
			PGHudWidget->SetupCharacterMinimap(num, ActorArray, allaicontrollers);

		bIsMiniMap = true;
	}
}

void APGPlayerCharacter::ChangeMiniMapSize()
{
	if (PGHudWidget)
	{
		APGPlayerController* playerController = Cast<APGPlayerController>(GetController());
		if (playerController)
		{
			if (bIsMiniMap)
			{

				bIsMiniMap = false;
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);
			}
			else
			{
				bIsMiniMap = true;
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
			}

			playerController->ChangeMiniMap(bIsMiniMap);

			PGHudWidget->ChangeMiniMapSize(bIsMiniMap);
		}
		
	}

}

void APGPlayerCharacter::SetupPlayerLevel(int32 level)
{
	StatComponent->SetUpPlayerLevel(level);
}

int32 APGPlayerCharacter::GetPlayerCharacterLevel()
{

	return StatComponent->GetCurrentLevel();
}

void APGPlayerCharacter::PlayerAddEXP()
{
	KOCount++; 

	FKoCountChanged.Broadcast(KOCount);

	StatComponent->AddEXP();
}

void APGPlayerCharacter::PlayLevelUpEffet(int32 currentlevel)
{

}

void APGPlayerCharacter::UpdateGameState(bool bIsclear)
{
	bIsGameStated = true;
	if (PGHudWidget)
	{

		PGHudWidget->UpdateGameState(bIsclear);



		if (!bIsclear)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
		}
	}
}

void APGPlayerCharacter::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
		if (OtherCharacter)
		{

			
			FVector Direction = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			Direction.Z = 0.0f;
		
			float PushStrength = 500.0f; 

			
			OtherCharacter->GetCharacterMovement()->AddImpulse(Direction * PushStrength, true);

		}


	}
}

void APGPlayerCharacter::AttackSlowStart()
{
	//SetbIsSlowMotion(true);
	CustomTimeDilation = 0.8f;
}

void APGPlayerCharacter::AttackSlowEnd()
{
	//SetbIsSlowMotion(false);
	CustomTimeDilation = 1.0f;
}

void APGPlayerCharacter::AllTimelineSetting()
{
	
	 // 에임 타임라인
	UTimeLineWrapper* AimWrapper = NewObject<UTimeLineWrapper>(this);
	FOnTimelineFloat AimProgress;
	AimProgress.BindUFunction(this, FName("AimUpdate"));
	AimWrapper->Timeline.AddInterpFloat(AimCurve, AimProgress);
	

	// 공격 타임라인
	UTimeLineWrapper* AttackWrapper = NewObject<UTimeLineWrapper>(this);
	FOnTimelineFloat AttackProgress;
	AttackProgress.BindUFunction(this, FName("AttackCameraMove"));
	AttackWrapper->Timeline.AddInterpFloat(AttackCurve, AttackProgress);
	

	// 대쉬 타임라인
	UTimeLineWrapper* DashWrapper = NewObject<UTimeLineWrapper>(this);
	FOnTimelineFloat DashProgress;
	DashProgress.BindUFunction(this, FName("DashCameraMove"));
	DashWrapper->Timeline.AddInterpFloat(DashCurve, DashProgress);
	

	// 바디가드 타임라인
	UTimeLineWrapper* BodyguardWrapper = NewObject<UTimeLineWrapper>(this);
	FOnTimelineFloat BodyguardProgress;
	BodyguardProgress.BindUFunction(this, FName("BodyGuardCameraMove"));
	BodyguardWrapper->Timeline.AddInterpFloat(BodyGuardCurve, BodyguardProgress);

	// 필드체인지 타임라인
	UTimeLineWrapper* FieldChangeWrapper = NewObject<UTimeLineWrapper>(this);
	FOnTimelineFloat FieldChangeProgress;
	FieldChangeProgress.BindUFunction(this, FName("FieldChangeCameraMove"));
	FieldChangeWrapper->Timeline.AddInterpFloat(FieldChangeCurve, FieldChangeProgress);

	//아버 브레이크 타임라인
	UTimeLineWrapper* ArmorBreakWrapper = NewObject<UTimeLineWrapper>(this);
	FOnTimelineFloat ArmorBreakProgress;
	ArmorBreakProgress.BindUFunction(this, FName("ArmorBreakCameraMove"));
	ArmorBreakWrapper->Timeline.AddInterpFloat(ArmorBreakCurve, ArmorBreakProgress);

	FOnTimelineEvent TimelineFinished;
	TimelineFinished.BindUFunction(this, FName("OnArmorBreakTimelineFinished"));
	ArmorBreakWrapper->Timeline.SetTimelineFinishedFunc(TimelineFinished);
	
	AllCameraTimeline.Add(ECameraMoveType::AimCamera, AimWrapper);
	AllCameraTimeline.Add(ECameraMoveType::AttackCamera, AttackWrapper);
	AllCameraTimeline.Add(ECameraMoveType::DashCamera, DashWrapper);
	AllCameraTimeline.Add(ECameraMoveType::BodyGuardCamera, BodyguardWrapper);
	AllCameraTimeline.Add(ECameraMoveType::FieldChangeCamera, FieldChangeWrapper);
	AllCameraTimeline.Add(ECameraMoveType::ArmorBreakCamera, ArmorBreakWrapper);


}

void APGPlayerCharacter::AllTimelineTick(float dt)
{
	AllCameraTimeline[ECameraMoveType::AimCamera]->Timeline.TickTimeline(dt);
	AllCameraTimeline[ECameraMoveType::DashCamera]->Timeline.TickTimeline(dt);
	AllCameraTimeline[ECameraMoveType::AttackCamera]->Timeline.TickTimeline(dt);
	AllCameraTimeline[ECameraMoveType::BodyGuardCamera]->Timeline.TickTimeline(dt);
	AllCameraTimeline[ECameraMoveType::FieldChangeCamera]->Timeline.TickTimeline(dt);
	AllCameraTimeline[ECameraMoveType::ArmorBreakCamera]->Timeline.TickTimeline(dt);
	
}


void APGPlayerCharacter::AllTimelineStop(ECameraMoveType cameramovetype)
{
	for (TPair<ECameraMoveType, UTimeLineWrapper*>& Elem : AllCameraTimeline)
	{
		
		UTimeLineWrapper* Timeline = Elem.Value;
		if (Timeline->Timeline.IsPlaying()) 
		{
			Timeline->Timeline.Stop();
		}
	}
}




void APGPlayerCharacter::AttackCameraMove(float dt)
{

	float AimX;
	float AimY;
	float AimZ;

	if (!bIsReversed)
	{
		AimX = FMath::Lerp(CameraCurrentLocation.X, 150.0f, dt);
		AimY = FMath::Lerp(CameraCurrentLocation.Y, 75.0f, dt);
		AimZ = FMath::Lerp(CameraCurrentLocation.Z, -50.0f, dt);
	}
	else
	{
		AimX = FMath::Lerp(CameraCurrentLocation.X, 0.0f, dt);
		AimY = FMath::Lerp(CameraCurrentLocation.Y, 0.0f, dt);
		AimZ = FMath::Lerp(CameraCurrentLocation.Z, 0.0f, dt);
	}
	

	Camera->SetRelativeLocation(FVector(AimX, AimY, AimZ));
	Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	if (DynamicMaterialInstance)
		DynamicMaterialInstance->SetScalarParameterValue(FName("Alpha"), 0.0f);
}

void APGPlayerCharacter::DashCameraMove(float dt)
{
	float AimX;
	float AimY;
	float AimZ;

	if (!bIsReversed)
	{
		AimX = FMath::Lerp(CameraCurrentLocation.X, -50.0f, dt);
		AimY = FMath::Lerp(CameraCurrentLocation.Y, 25.0f, dt);
	}
	else
	{
		AimX = FMath::Lerp(CameraCurrentLocation.X, 0.0f, dt);
		AimY = FMath::Lerp(CameraCurrentLocation.Y, 0.0f, dt);
	}
	AimZ = FMath::Lerp(CameraCurrentLocation.Z, 0.0f, dt);
	
	Camera->SetRelativeLocation(FVector(AimX, AimY, AimZ));
	Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	if (DynamicMaterialInstance)
		DynamicMaterialInstance->SetScalarParameterValue(FName("Alpha"), 0.0f);
}

void APGPlayerCharacter::BodyGuardCameraMove(float dt)
{
	float AimX;
	float AimY;
	float AimZ;
	float RotatePitch;
	float Alpha;
	if (!bIsReversed)
	{
		AimX = FMath::Lerp(CameraCurrentLocation.X, -500.0f, dt);
		AimZ = FMath::Lerp(CameraCurrentLocation.Z, 400.0f, dt);
		RotatePitch = FMath::Lerp(CameraCurrentRotator.Pitch, -25.0f, dt);
		Alpha = FMath::Lerp(0.0f, 1.0f, dt);
	}
	else
	{
		AimX = FMath::Lerp(CameraCurrentLocation.X, 0.0f, dt);
		AimZ = FMath::Lerp(CameraCurrentLocation.Z, 0.0f, dt);
		RotatePitch = FMath::Lerp(CameraCurrentRotator.Pitch, 0.0f, dt);
		Alpha = FMath::Lerp(1.0f, 0.0f, dt);
	}
	AimY = FMath::Lerp(CameraCurrentLocation.Y, 0.0f, dt);


	Camera->SetRelativeLocation(FVector(AimX, AimY, AimZ));
	Camera->SetRelativeRotation(FRotator(RotatePitch, 0.0f, 0.0f));
	if(DynamicMaterialInstance)
		DynamicMaterialInstance->SetScalarParameterValue(FName("Alpha"), Alpha);
}

void APGPlayerCharacter::FieldChangeCameraMove(float dt)
{
	
	float AimX;
	float AimY;
	float AimZ;
	float RotatePitch;
	
	float Add = 1.0f;
	if (CustomTimeDilation < 1.0f)
	{
		
		Add = 100.0f;
		
	}

	if (!bIsReversed)
	{
		
		AimX = FMath::FInterpTo(CameraCurrentLocation.X, -300.0f, dt, Add);
		AimZ = FMath::FInterpTo(CameraCurrentLocation.Z, 800.0f, dt, Add);
		RotatePitch = FMath::FInterpTo(CameraCurrentRotator.Pitch, -40.0f, dt, Add);
		
	}
	else
	{
		AimX = FMath::FInterpTo(CameraCurrentLocation.X, 0.0f, dt, Add);
		AimZ = FMath::FInterpTo(CameraCurrentLocation.Z, 0.0f, dt, Add);
		RotatePitch = FMath::FInterpTo(CameraCurrentRotator.Pitch, 0.0f, dt, Add);
		
	}
	AimY = FMath::FInterpTo(CameraCurrentLocation.Y, 0.0f, dt, Add);


	Camera->SetRelativeLocation(FVector(AimX, AimY, AimZ));
	Camera->SetRelativeRotation(FRotator(RotatePitch, 0.0f, 0.0f));
}

void APGPlayerCharacter::ArmorBreakCameraMove(float dt)
{

	if (!ArmorBreakElite)return;

	FVector EliteLocation = ArmorBreakElite->GetActorLocation();

	FVector Direction = (CameraWorldLocation - EliteLocation).GetSafeNormal();


	FVector CameraFocusLocation = EliteLocation + (Direction * 200.0f)+FVector::UpVector*25.0f;
	FRotator CameraFocusRotation = (EliteLocation - CameraWorldLocation).Rotation();
	

	FVector NEWCameraLocation = FMath::VInterpTo(CameraWorldLocation, CameraFocusLocation, dt, 1.0f);
	FRotator NEWCameraRotation= FMath::RInterpTo(CameraWorldRotation, CameraFocusRotation, dt, 1.0f);

	

	Camera->SetWorldLocationAndRotation(NEWCameraLocation, NEWCameraRotation);
}

void APGPlayerCharacter::OnArmorBreakTimelineFinished()
{
	APGPlayerController* playerController = Cast<APGPlayerController>(GetController());
	if (playerController)
	{
		playerController->SetIgnoreLookInput(false);
		playerController->SetIgnoreMoveInput(false);
	}
	ArmorBreakElite = nullptr;
	Camera->SetRelativeLocation(FVector::Zero());
	Camera->SetRelativeRotation(FRotator::ZeroRotator);


	
}

void APGPlayerCharacter::StartSetCameraMoveSetting(bool bisreversed, ECameraMoveType cameramovetype)
{
	if (AllCameraTimeline[ECameraMoveType::ArmorBreakCamera]->Timeline.IsPlaying())return;

	bIsReversed = bisreversed;
	CameraCurrentLocation = Camera->GetRelativeLocation();
	CameraCurrentRotator = Camera->GetRelativeRotation();
	AllTimelineStop(cameramovetype);
	AllCameraTimeline[cameramovetype]->Timeline.PlayFromStart();
}

bool APGPlayerCharacter::CanPlayerProtect(APawn* pawn)
{
	return AIBodyGuardComponent->CanPlayerProtect(pawn);
}

AActor* APGPlayerCharacter::SetPlayerProtectPawn(APawn* pawn)
{
	return AIBodyGuardComponent->SetPlayerProtectPawn(pawn);
}


void APGPlayerCharacter::BodyGuardOptionsClick(int32 optionnum,uint8 optionGauge)
{
	
	if (AIBodyGuardComponent->UseBGOptionGauge(optionGauge))
	{
		CloseBodyGuardOption();
		AIBodyGuardComponent->BodyGuardOptionsClick(optionnum);
	}
	
}

void APGPlayerCharacter::BoyGuardOptionHover(int32 optionnum)
{
	AIBodyGuardComponent->BoyGuardOptionHover(optionnum);
}

void APGPlayerCharacter::HoverUp()
{
	AIBodyGuardComponent->DestroyAllPreviewActors();
}

void APGPlayerCharacter::DeletePlayerProtectPawn(APawn* pawn)
{
	AIBodyGuardComponent->DeletePlayerProtectPawn(pawn);
}

void APGPlayerCharacter::ShowBodyGuardOption()
{

	//SLOG(TEXT("SHowBodyguard"));
	if (bShowBodyGuardOption)
	{
		return;
	}
	bShowBodyGuardOption = true;
	bIsGlobalTimeSlow = bShowBodyGuardOption;
	StartSetCameraMoveSetting(false,ECameraMoveType::BodyGuardCamera);
	
	APGPlayerController* playerController = Cast<APGPlayerController>(GetController());
	if (playerController)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);

		playerController->ShowBodyGuardOption(bShowBodyGuardOption);

		PGHudWidget->ChangeBodyGuardOptionSize(bShowBodyGuardOption);
	}

}


void APGPlayerCharacter::CloseBodyGuardOption()
{
	if (!bShowBodyGuardOption)
	{
		return;
	}
	bShowBodyGuardOption = false;
	bIsGlobalTimeSlow = bShowBodyGuardOption;
	StartSetCameraMoveSetting(true, ECameraMoveType::BodyGuardCamera);
	
	APGPlayerController* playerController = Cast<APGPlayerController>(GetController());
	if (playerController)
	{
	
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		

		playerController->ShowBodyGuardOption(bShowBodyGuardOption);

		PGHudWidget->ChangeBodyGuardOptionSize(bShowBodyGuardOption);
	}

}

void APGPlayerCharacter::SetPostProcessMaterial()
{
	if (PostProcessMaterial)
	{

		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(PostProcessMaterial, this);

		if (Camera)
		{
			Camera->PostProcessSettings.AddBlendable(DynamicMaterialInstance, 1.0f);
		}
	}
}

void APGPlayerCharacter::StartFieldChangedCamera(bool start)
{

	APGPlayerController* playerController = Cast<APGPlayerController>(GetController());
	if (playerController)
	{
		bIsGlobalTimeSlow = start;

		
		StartSetCameraMoveSetting(!start, ECameraMoveType::FieldChangeCamera);


		if (start)
		{
			if (CutSceneCamera->IsActive())
			{
				ChangeViewTarget(false);
			}
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);

		}
		else
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
			PlayCharacterVoice(ECharacterVoiceType::TakeField);
		}

		playerController->SetIgnoreLookInput(start);
	
		
	}
}

void APGPlayerCharacter::ArmorBreakCameraFocus(AActor* eliteNPC)
{
	if (!eliteNPC) return;
	ArmorBreakElite = eliteNPC;
	
	CameraWorldLocation = Camera->GetComponentLocation();
	CameraWorldRotation = Camera->GetComponentRotation();

	OnSlowOVerlapToNPC(1.0f, nullptr);

	APGPlayerController* playerController = Cast<APGPlayerController>(GetController());
	if (playerController)
	{

		playerController->SetIgnoreLookInput(true);
		playerController->SetIgnoreMoveInput(true);
	}

	StartSetCameraMoveSetting(false, ECameraMoveType::ArmorBreakCamera);
	
}

bool APGPlayerCharacter::HasPlayerController()
{
	APGPlayerController* playerController = Cast<APGPlayerController>(GetController());
	if (playerController)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void APGPlayerCharacter::SetInExecutionRange(bool InRange, AActor* eliteNPC)
{
	bIsExecutionRange = InRange;
	IExecutionEliteNPCInterface* elite = Cast<IExecutionEliteNPCInterface>(eliteNPC);
	if (elite)
	{
		ExecutionElite = bIsExecutionRange ? eliteNPC : nullptr;
		
	}
	
}

void APGPlayerCharacter::OnExecution()
{
	if (ExecutionElite)
	{

		AActor* eliteNPC = ExecutionElite->OnExecutionStart(this);
		if (eliteNPC&& AllMontage[ExcuisonAttackMontage])
		{
			
			OnSlowOVerlapToNPC(AllMontage[ExcuisonAttackMontage]->GetPlayLength(), eliteNPC);
		}
		StartExecution();
	}
}

void APGPlayerCharacter::StartExecution()
{
	APGPlayerController* playerController = Cast<APGPlayerController>(GetController());
	if (playerController)
	{
		DisableInput(playerController);
	}
	bIsExecution = true;
	AttackComponent->SetbIsGodMode(bIsExecution);
	PlayExecutionMontage();
	StartExecutionSequence();
}

void APGPlayerCharacter::PlayExecutionMontage()
{

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AllMontage[ExcuisonAttackMontage], 1.0f);
	FOnMontageEnded ExcuteEndDelegate;
	ExcuteEndDelegate.BindUObject(this, &APGPlayerCharacter::EndExecuitionMontage);
	AnimInstance->Montage_SetEndDelegate(ExcuteEndDelegate, AllMontage[ExcuisonAttackMontage]);
}


void APGPlayerCharacter::EndExecuitionMontage(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	APGPlayerController* playerController = Cast<APGPlayerController>(GetController());
	if (playerController)
	{
		EnableInput(playerController);
	}
	bIsExecutionRange = false;
	bIsExecution = false;
	AttackComponent->SetbIsGodMode(bIsExecution);

	PlayCharacterVoice(ECharacterVoiceType::KillElite);
	
	
}

void APGPlayerCharacter::StartExecutionSequence()
{
	
	ALevelSequenceActor* LevelSequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>(ExecutionLevelSequenceClass, GetActorLocation(),
		GetActorRotation());

	if (LevelSequenceActor)
	{
		// 레벨 시퀀스 설정
		if (ExecutionLevelSequence)
		{
			LevelSequenceActor->SetSequence(ExecutionLevelSequence);

			ULevelSequencePlayer* LevelSequencePlayer = LevelSequenceActor->GetSequencePlayer();

			// 컷씬 재생
			if (LevelSequencePlayer)
			{
				FMovieSceneObjectBindingID id = ExecutionLevelSequence->FindBindingByTag(TEXT("Character"));
				if (id.IsValid())
				{

					LevelSequenceActor->SetBinding(id, { this });

				}

				ChangeViewTarget(true);

				LevelSequencePlayer->Play();

				LevelSequencePlayer->OnFinished.AddDynamic(this, &ThisClass::FinishExecutionSequence);
			}
		}
	}
}

void APGPlayerCharacter::FinishExecutionSequence()
{
	ChangeViewTarget(false);
}

bool APGPlayerCharacter::GetExcution()
{
	return bIsExecution;
}

void APGPlayerCharacter::EyeBlinkStart()
{
	EyeBlinkTimeline.PlayFromStart();
}

float APGPlayerCharacter::GetEyeBlink()
{
	return EyeBlinkValue;
}

void APGPlayerCharacter::SetEyeBlinkTimeline()
{
	
	FOnTimelineFloat EyeBlinkProgress;
	EyeBlinkProgress.BindUFunction(this, FName("EyeBlinkUpdate"));
	EyeBlinkTimeline.AddInterpFloat(EyeBlinkCurve, EyeBlinkProgress);

	FOnTimelineEvent EyeBlinkTimelineFinished;
	EyeBlinkTimelineFinished.BindUFunction(this, FName("EyeBlinkFinished"));
	EyeBlinkTimeline.SetTimelineFinishedFunc(EyeBlinkTimelineFinished);
}

void APGPlayerCharacter::EyeBlinkUpdate(float dt)
{
	
	EyeBlinkValue = FMath::FInterpTo(0.0f, 1.0f, dt,1.0f);
	
}

void APGPlayerCharacter::EyeBlinkFinished()
{
	EyeBlinkTimeline.PlayFromStart();
}

USoundBase* APGPlayerCharacter::GetCharacterBGM()
{
	UPlayerCharacterDataAsset* playerdata = Cast<UPlayerCharacterDataAsset>(baseCharacterData);
	if (playerdata)
	{
		return playerdata->CharacterBGM;
	}
	return nullptr;
}

void APGPlayerCharacter::PlayCharacterVoice(ECharacterVoiceType VoiceType)
{
	if (!VoiceDataAsset)
		return;

	TSoftObjectPtr<USoundBase> SelectedVoice = VoiceDataAsset->GetVoiceByType(VoiceType);
	if (SelectedVoice.IsNull())
		return;

	//  데이터 에셋에서 우선순위를 가져옴
	int32 Priority = VoiceDataAsset->GetVoicePriority(VoiceType);



	if (VoiceComponent->IsPlaying())
	{
		// 현재 재생 중인 음성과 우선순위를 비교
		if (VoiceComponent->Priority<= Priority)
		{
			VoiceComponent->Stop();
		}
		else
		{
			return; // 현재 재생 중인 음성이 더 중요하면 새로운 음성을 재생하지 않음
		}
	}
	if (SelectedVoice.IsValid() == false)
	{
		SelectedVoice.LoadSynchronous(); // 만약 로드되지 않았다면 즉시 로드
	}

	if (SelectedVoice.IsValid())
	{
		
		VoiceComponent->SetSound(SelectedVoice.Get());
		VoiceComponent->Priority = Priority;
		VoiceComponent->Play();
	}
	else
	{
		SLOG(TEXT("Failed Load PlayerVoice"));
	}
	/*{
		
		SelectedVoice->([this, Priority](USoundBase* LoadedSound)
			{
				if (LoadedSound)
				{
					VoiceComponent->SetSound(LoadedSound);
					VoiceComponent->SetFloatParameter("Priority", Priority);
					VoiceComponent->Play();
				}
			});
	}*/
}

























