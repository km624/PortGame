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
		// �������°Ŵ� �̵������ faling��������� üũ�ϸ� ��
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
		return 0.0f; // �ӵ��� ������ ������ 0
	}

	// �ӵ��� ����ȭ�Ͽ� ���� ���
	FVector ForwardVector = ActorRotation.Vector(); // ����� Forward ����
	FVector RightVector = FVector::CrossProduct(FVector::UpVector, ForwardVector); // ����� Right ����

	// �ӵ��� ���� �� ���� Dot Product ���
	float ForwardDot = FVector::DotProduct(ForwardVector, newVelocity.GetSafeNormal());
	float RightDot = FVector::DotProduct(RightVector, newVelocity.GetSafeNormal());

	// ���� ��� (��ũź��Ʈ�� �̿�)
	float Angle = FMath::RadiansToDegrees(FMath::Atan2(RightDot, ForwardDot));

	return Angle;
}

