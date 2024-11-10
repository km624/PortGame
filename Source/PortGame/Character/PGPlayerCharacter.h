// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PGBaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "Interface/PGHudWidgetInterface.h"
#include "PGPlayerCharacter.generated.h"

/**
 * 
 */

UENUM()
enum class EControlData : uint8
{
	Base,
	Aim

};

UCLASS()
class PORTGAME_API APGPlayerCharacter : public APGBaseCharacter,public IPGHudWidgetInterface
{
	GENERATED_BODY()
public:

	APGPlayerCharacter();

	

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;



	void SetCharacterData(EControlData DataName);


protected:
	UPROPERTY(EditAnywhere, Category = ControlData, Meta = (AllowPrivateAccess = "true"))
	TMap<EControlData, class UPGCharacterData*>  ControlDataManager;

	EControlData CurrentControlData;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction>AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction>AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction>ReloadAction;

	//�Ű����� �ޱ� ���� ��� ��Ŭ���
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);

	//����
	void Attack();

	void OnGoingAttack();

	void ReleasedAttack();

	//���� ������
	void PressAim();

	void OnGoingAim();
	//���� ������
	void ReleasedAim();

	void PressReload();

	//���� 

public:

	void SetAimTargetLocation();

//������ ī�޶�
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim")
	TObjectPtr<class UCurveFloat> AimCurve;

	FTimeline AimTimeline;

	UFUNCTION()
	void AimUpdate(float deltaTime);

	
protected:
	virtual void SetUpHudWidget(class UPGHudWidget* hudWidget) override;




	
};
