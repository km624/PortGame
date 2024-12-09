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
	//���� �ɶ� �ѹ� ȣ��
	virtual void NativeInitializeAnimation() override;
	//��� ȣ��
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//ĳ���� ���� ��� ��ü
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	//�ִϸ��̼ǿ� �ʿ��� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	//MAx Walk speed ������ �ö�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	//�����̴� �� �����ִ��� 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	//�����������ΰ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	//���� ����������
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
