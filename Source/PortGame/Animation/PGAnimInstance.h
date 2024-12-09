// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PGAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPGAnimInstance();

protected:
	//생성 될때 한번 호출
	virtual void NativeInitializeAnimation() override;
	//계속 호출
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//캐릭터 정보 담는 객체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	//애니메이션에 필요한 변수들
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	//MAx Walk speed 까지만 올라감
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	//움직이는 지 쉬고있는지 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	//떨어지는중인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	//현재 점프중인지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float CharacterDirection;

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim)
	uint8 bIsAiming : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim)
	float AimOffsetPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Reload)
	uint8 bIsReloading : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsDead : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = NikkeSkill)
	uint8 bIsNikkeSkill : 1;

	
	

	
	


	
};
