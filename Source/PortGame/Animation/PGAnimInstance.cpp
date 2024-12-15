// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PGAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "GameFramework/Character.h"
#include "Character/PGPlayerCharacter.h"
#include "Character/PGBaseCharacter.h"
//#include "Kismet/KismetMathLibrary.h"
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
	BaseCharacter = Cast<APGBaseCharacter>(Owner);
	
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();

	}
}

void UPGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{

	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		// 떨어지는거는 이동모드의 faling모드인지만 체크하면 됨
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);

		//CharacterDirection = CalculateDirection(Owner->GetVelocity(), Owner->GetActorRotation());
		CharacterDirection = CalculateDirectionAlternative(Owner->GetVelocity(), Owner->GetActorRotation());

	
	}
	
	if (IsValid(BaseCharacter))
	{
		bIsAiming = BaseCharacter->GetCurrentIsAiming();
		bIsReloading = BaseCharacter->GetCurrentIsReloading();
		bIsDead = BaseCharacter->GetbIsDead();
		bIsNikkeSkill = BaseCharacter->GetbIsNikkeSkill();
		bIsRifle = BaseCharacter->GetCurrentIsRifle();
	}


	if (bIsAiming)
		AimOffsetPitch = BaseCharacter->ReturnAimOffset();
	else
		AimOffsetPitch = 0.0f;

	/*if (IsValid(BaseCharacter))
	{
		bIsReloading = BaseCharacter->GetCurrentIsReloading();
	}

	if (IsValid(BaseCharacter))
	{
		bIsDead = BaseCharacter->GetbIsDead();
	}
	if (IsValid(BaseCharacter))
	{
		bIsNikkeSkill = BaseCharacter->GetbIsNikkeSkill();
	}*/
	
	
}

float UPGAnimInstance::CalculateDirectionAlternative(const FVector& newVelocity, const FRotator& ActorRotation)
{
	if (newVelocity.IsNearlyZero())
	{
		return 0.0f; // 속도가 없으면 방향은 0
	}

	// 속도를 정규화하여 방향 계산
	FVector ForwardVector = ActorRotation.Vector(); // 배우의 Forward 벡터
	FVector RightVector = FVector::CrossProduct(FVector::UpVector, ForwardVector); // 배우의 Right 벡터

	// 속도와 로컬 축 간의 Dot Product 계산
	float ForwardDot = FVector::DotProduct(ForwardVector, newVelocity.GetSafeNormal());
	float RightDot = FVector::DotProduct(RightVector, newVelocity.GetSafeNormal());

	// 방향 계산 (아크탄젠트를 이용)
	float Angle = FMath::RadiansToDegrees(FMath::Atan2(RightDot, ForwardDot));

	return Angle;
}

