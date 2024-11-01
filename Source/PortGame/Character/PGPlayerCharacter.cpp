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

APGPlayerCharacter::APGPlayerCharacter()
{
	static ConstructorHelpers::FObjectFinder<UPGCharacterData> CharacterData
	(TEXT("/Script/PortGame.PGCharacterData'/Game/PortGame/Input/PG_InputData.PG_InputData'"));
	if (CharacterData.Object)
		ControlDataManager.Add(EControlData::Base, CharacterData.Object);
	else
		UE_LOG(LogTemp, Warning, TEXT("ERRor  failed"));
	
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
	else
		UE_LOG(LogTemp, Warning, TEXT("ERRor  failed"));
	static ConstructorHelpers::FObjectFinder<UInputAction> Move(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Move.IA_Move'"));
	if (Move.Object)
	{
		MoveAction = Move.Object;
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("ERRor  failed"));
	static ConstructorHelpers::FObjectFinder<UInputAction> Look(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Look.IA_Look'"));
	if (Look.Object)
	{
		LookAction = Look.Object;
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("ERRor  failed"));

	static ConstructorHelpers::FObjectFinder<UInputAction> Attack(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Attack.IA_Attack'"));
	if (Attack.Object)
	{
		AttackAction = Attack.Object;
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("ERRor  failed"));

	//에임 바인딩
	static ConstructorHelpers::FObjectFinder<UInputAction> Aim(TEXT("/Script/EnhancedInput.InputAction'/Game/PortGame/Input/InputAction/IA_Aim.IA_Aim'"));
	if (Aim.Object)
	{
		AimAction = Aim.Object;
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("ERRor  failed"));

	//무기
	//스켈레탈 컴포넌트 추가
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("weaponRifleSocket"));

}

void APGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterData(CurrentControlData);

	EquidWeapon();

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
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APGPlayerCharacter::Attack);

		//Aiming
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &APGPlayerCharacter::PressAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APGPlayerCharacter::ReleasedAim);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
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

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APGPlayerCharacter::Attack()
{

	ComboStart();
	
}

void APGPlayerCharacter::PressAim()
{
	UE_LOG(LogTemp, Warning, TEXT("aim"));
	bIsAim = true;
}

void APGPlayerCharacter::ReleasedAim()
{
	UE_LOG(LogTemp, Warning, TEXT("aimOut"));
	bIsAim = false;
}

float APGPlayerCharacter::ReturnAimOffset()
{
	FRotator rtemp = GetActorRotation() - GetBaseAimRotation();


		rtemp.Normalize();

		float Direction = rtemp.Pitch; 
		float Direction2 = GetBaseAimRotation().Yaw;
		AimOffsetPitch = FMath::Clamp(Direction, -55.0f, 55.0f);

	return AimOffsetPitch;
}

void APGPlayerCharacter::EquidWeapon()
{
	Weapon->SetSkeletalMesh(WeaponMesh);

}




