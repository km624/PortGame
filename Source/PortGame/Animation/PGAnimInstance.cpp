// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PGAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "GameFramework/Character.h"
#include "Character/PGPlayerCharacter.h"
#include "Character/PGBaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "PortGame/PortGame.h"

//#include "Kismet/KismetAnimationLibrary.h"


UPGAnimInstance::UPGAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UPGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();

	}
}

void UPGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{

	Super::NativeUpdateAnimation(DeltaSeconds);

	APGBaseCharacter* BaseCharacter = Cast<APGBaseCharacter>(Owner);
	

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		// 떨어지는거는 이동모드의 faling모드인지만 체크하면 됨
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
		CharacterDirection = CalculateDirection(Owner->GetVelocity(), Owner->GetActorRotation());
		//UE_LOG(LogTemp, Warning, TEXT("%f"), GroundSpeed);
	}
	//APGBaseCharacter* BaseCharacter = Cast<APGBaseCharacter>(Owner);
	if (IsValid(BaseCharacter))
	{
		bIsAiming = BaseCharacter->GetCurrentIsAiming();
	}


	if (bIsAiming)
		AimOffsetPitch = BaseCharacter->ReturnAimOffset();
	else
		AimOffsetPitch = 0.0f;

	if (IsValid(BaseCharacter))
	{
		bIsReloading = BaseCharacter->GetCurrentIsReloading();
	}

	if (IsValid(BaseCharacter))
	{
		bIsDead = BaseCharacter->GetbIsDead();
	}
	
}

