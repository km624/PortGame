// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "PGCharacterData.h"
#include "Components/TimelineComponent.h"
#include "UI/PGHudWidget.h"
#include "Component/PGStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PortGame/PortGame.h"
#include "MotionWarpingComponent.h"




APGPlayerCharacter::APGPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("Motion_Warp"));
	
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
}

void APGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterData(CurrentControlData);


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

		EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Started, this, &APGPlayerCharacter::FindClosestEnemy);
	
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
	if (bIsTargetLock)
	{
		TargetLockOn(TargetActor, DeltaTime);
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
		
	}

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
	if (bIsAim)
	{
		//AimLocation = Camera->GetComponentLocation();
		bIsShoot = true;
	}
	AttackToComponent();
	
}

void APGPlayerCharacter::OnGoingAttack()
{
	if (bIsAim)
	{
		//AimLocation = Camera->GetComponentLocation();
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
	ReloadToWeapon();
}



void APGPlayerCharacter::AimUpdate(float deltaTime)
{
	
	//float Aim = FMath::Lerp( 300, 125, deltaTime);
	float AimX = FMath::Lerp(0, 100, deltaTime);
	float AimY = FMath::Lerp(0, 70, deltaTime);
	double SoccketOffsetY= FMath::Lerp(0, 45, deltaTime);
	double SoccketOffsetZ = FMath::Lerp(0, -20, deltaTime);
	//SpringArm->TargetArmLength = Aim;
	Camera->SetRelativeLocation(FVector(AimX, AimY, 0.0f));
	//SpringArm->SocketOffset.Set(0, SoccketOffsetY, SoccketOffsetZ);

}


void APGPlayerCharacter::SetUpHudWidget(UPGHudWidget* hudWidget)
{
	if (hudWidget)
	{
		hudWidget->SetUpWaidget(StatComponent->GetBaseStat(), StatComponent->GetModifierStat(), StatComponent->GetMaxHitGauge());
		UE_LOG(LogTemp, Warning, TEXT("hudwidget %f"), StatComponent->GetCurrentHp());
		hudWidget->UpdateHpBar(StatComponent->GetCurrentHp());
		hudWidget->UpdateHitGaugeBar(StatComponent->GetCurrentHitGauge());

		//델리게이트 바인딩
		StatComponent->OnStatChanged.AddUObject(hudWidget, &UPGHudWidget::SetUpWaidget);
		StatComponent->OnHpChanged.AddUObject(hudWidget, &UPGHudWidget::UpdateHpBar);
		StatComponent->OnHitGaugeChanged.AddUObject(hudWidget, &UPGHudWidget::UpdateHitGaugeBar);

		OnbIsAim.AddUObject(hudWidget, &UPGHudWidget::CorssHairEnable);
	}
}

void APGPlayerCharacter::FindClosestEnemy()
{
	if (bIsTargetLock)
	{
		TargetActor = NULL;
		bIsTargetLock = false;
		return;
	}
	
	TArray<FHitResult> OutHitResults;

	
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Target), false, this);

	// 스탯 컴포넌트에서 공격 범위, 반경, 데미지 값을 가져옴
	const float SerachRange = SearchDistance;
	const float SerachRadius = SearchDistance;
	

	// 공격 시작 지점 계산: 현재 액터의 위치에서 전방 벡터와 캡슐 반경을 이용
	//const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector Start = GetActorLocation() + GetControlRotation().Vector() * (SerachRadius);
		//+GetControlRotation().Vector() * (SerachRadius*0.5f);

	// 공격 끝 지점 계산: 시작 지점에서 전방 벡터 방향으로 공격 범위 만큼 이동
	//const FVector End = Start + GetActorForwardVector()*SearchDistance;
		const FVector End = GetActorLocation() + GetControlRotation().Vector() * (SerachRadius);
		//+GetControlRotation().Vector() * (SerachRadius * 0.25f);

	// SweepMultiByChannel을 사용하여 충돌 감지 수행
	bool HitDetected = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(SerachRadius),
		Params
	);

	// 히트가 감지된 경우 처리
	if (HitDetected)
	{
		AActor* ClosestEnemy = nullptr;
		float MinDistanceSq = FMath::Square(SerachRadius*2);
		// 히트된 모든 결과를 순회
		for (const FHitResult& Hit : OutHitResults)
		{
			AActor* Actor = Hit.GetActor();
			if (Actor)
			{
				if (Actor && Actor->ActorHasTag(TEXT("Enemy")))
				{
					
					// 캐릭터와 적 사이의 거리 계산 (제곱 거리 사용하여 성능 최적화)
					float DistanceSq = FVector::DistSquared(GetActorLocation(), Actor->GetActorLocation());
					if (DistanceSq < MinDistanceSq)
					{
						MinDistanceSq = DistanceSq;
						ClosestEnemy = Actor;
						TargetActor = Actor;
						bIsTargetLock = true;
					}
				}
			}
		
		}
		if (ClosestEnemy)
		{
			SLOG( TEXT("%s"), *ClosestEnemy->GetActorLabel());
			
		}
			
		
	}
	else
		TargetActor = nullptr;



#if ENABLE_DRAW_DEBUG

	// 캡슐의 중앙 위치 계산: 시작점과 끝점의 중간
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	// 캡슐의 절반 높이 계산: 검색 범위의 절반
	float CapsuleHalfHeight = SerachRange * 0.5f;
	// 캡슐의 회전 계산: 전방 벡터 방향으로 회전
	FRotator CapsuleRotation = GetActorRotation();

	// 히트 감지 여부에 따라 색상 설정
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	// 디버그 캡슐의 지속 시간 설정 (1초)
	float DebugLifeTime = 1.0f;

	// 디버그 캡슐 그리기
	DrawDebugCapsule(GetWorld(),
		CapsuleOrigin,
		CapsuleHalfHeight,
		SerachRadius,
		CapsuleRotation.Quaternion(),
		DrawColor,
		false,
		DebugLifeTime
	);
	
#endif

	
}

void APGPlayerCharacter::TargetLockOn(AActor* targetActor,float dt)
{
	
	if (CharcterTargetDistance(targetActor) > SearchDistance)
	{
		TargetActor = NULL;
		bIsTargetLock = false;
		return;
	}
	SetMotionWarpingLocation(targetActor->GetActorLocation());
	FRotator newRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetActor->GetActorLocation());
	
	FRotator newnewRotator = FMath::RInterpTo(GetControlRotation(), newRotator, dt,3.0f);
	GetController()->SetControlRotation(newnewRotator);
}

float APGPlayerCharacter::CharcterTargetDistance(AActor* targetActors)
{
	// 내 액터의 위치 가져오기
	FVector MyLocation = GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();
	float Distance = FVector::Dist(MyLocation, TargetLocation);
	SLOG(TEXT("Distance :%f"), Distance);
	return Distance;
}

void APGPlayerCharacter::SetMotionWarpingLocation(FVector targetPos)
{
	if (bIsTargetLock)
	{
		if (CharcterTargetDistance(TargetActor) > 300.0f)
		{
			MotionWarpingComponent->RemoveWarpTarget(TEXT("Target"));
			return;
		}

	}
	
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(TEXT("Target"), targetPos);
	
}







