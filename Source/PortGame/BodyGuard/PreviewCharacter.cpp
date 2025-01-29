// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyGuard/PreviewCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimationAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

APreviewCharacter::APreviewCharacter()
{
	//ÃÊ±â ¼ÂÆÃ  - Ä¸½¶ /¹«ºê¸ÕÆ® ÄÄÆ÷³ÍÆ®
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("OverlapAll"));

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
}


void APreviewCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}



void APreviewCharacter::SetAnimation(UAnimationAsset* animasset)
{
	if (GetMesh() && animasset)
	{
		UAnimSequence* AnimSequence = Cast<UAnimSequence>(animasset);
		if (AnimSequence)
		{
			AnimSequence->EnableRootMotionSettingFromMontage(true,ERootMotionRootLock::RefPose);
		}
		
		GetMesh()->PlayAnimation(animasset, true);

		
		GetMesh()->SetPlayRate(4.0f);
	}
	
}




