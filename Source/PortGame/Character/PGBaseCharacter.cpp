// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	//GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Mesh 위치랑 회전 해줌 안맞아서
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);


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

}



