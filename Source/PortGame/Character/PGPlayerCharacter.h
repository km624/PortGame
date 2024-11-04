// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PGBaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "PGPlayerCharacter.generated.h"

/**
 * 
 */

UENUM()
enum class EControlData : uint8
{
	Base,
	Two

};

UCLASS()
class PORTGAME_API APGPlayerCharacter : public APGBaseCharacter
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

	//매개변수 받기 위해 헤더 인클루드
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);

	//공격
	void Attack();

	//에임 누르기
	void PressAim();
	//에임 땟을때
	void ReleasedAim();

	//에임 

//에임중 카메라
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim")
	TObjectPtr<class UCurveFloat> AimCurve;

	FTimeline AimTimeline;

	UFUNCTION()
	void AimUpdate(float deltaTime);

	




	//무기 
//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
//	TObjectPtr<class USkeletalMeshComponent> Weapon;
//	
//	void EquidWeapon();
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
//	TObjectPtr<USkeletalMesh>WeaponMesh;
//
//	
//	//총 발사 구현
//public: 
//	int32 ammoMaxCount = 10;
//
//	int32 currentammo;
//	float reloadingTime = 3.0f;
//
//	float ShootInterval = 1.0f;
//
//	float traceDistance = 1000.0f;
//
//	void StartFire();
//
//	void StopFire();
//
//	void Reloading();
//
//	FTimerHandle FireTimerHandle;
//	FTimerHandle ReloadTimerHandle;
//	//UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
//	//class USkeletalMeshComponent* WeaponSkeletalMeshComponent;
//
//	
//	void FireWithLineTrace();
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim")
//	TObjectPtr<class UAnimMontage> ReloadMwontage;
};
