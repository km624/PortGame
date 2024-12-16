// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PGAIBaseCharacter.h"
//#include "Character/PGBaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "Interface/PGHudWidgetInterface.h"
#include "Interface/AttackHitStopInterface.h"
#include "Interface/AITargetPlayerInterface.h"
#include "PGPlayerCharacter.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnDashDelegate);
UENUM()
enum class EControlData : uint8
{
	Base,
	Aim
};
UCLASS()
class PORTGAME_API APGPlayerCharacter : public APGAIBaseCharacter, public IPGHudWidgetInterface ,public IAttackHitStopInterface, public IAITargetPlayerInterface
{

	GENERATED_BODY()

public:

	APGPlayerCharacter();

	

protected:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupCharacterData(class UBaseCharacterDataAsset* characterdata) override;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction>TargetAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction>TargetSideAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction>DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction>SkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction>UltiSkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction>OneChangeCharacterAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction>TwoChangeCharacterAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction>ThreeChangeCharacterAction;



	//매개변수 받기 위해 헤더 인클루드
	void Move(const struct FInputActionValue& Value);
	void SetNoneMove();

	void Look(const struct FInputActionValue& Value);

	//공격
	void Attack();

	void OnGoingAttack();

	void ReleasedAttack();

	
	//공격 - > 총
	void PressAim();

	void OnGoingAim();
	
	void ReleasedAim();

	void PressReload();

	//에임 


//에임중 카메라
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aim")
	TObjectPtr<class UCurveFloat> AimCurve;

	FTimeline AimTimeline;

	UFUNCTION()
	void AimUpdate(float deltaTime);

	//데미지
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//HUD
protected:
	virtual void SetUpHudWidget(class UPGHudWidget* hudWidget) override;


	//타겟팅
protected:

	void FindClosestEnemyToComp();

	void FindSideEnemyToComp(const struct FInputActionValue& Value);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTargetingComponent> TargetingComponent;


	//모션워핑
public:

	void SetMotionWarpingLocation(FVector targetPos);

	void ResetMotionWarping();


	//공격중 회전구현
public:
	FORCEINLINE void SetbIsAttackRotation(bool bisRot) { bIsAttackRotation = bisRot;}

protected:

	void SetAttackRotation(float dt);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	FVector AttackVector;

	uint8 bIsAttackRotation : 1;

	uint8 bIsMoving : 1;

	//공격 슬로우모션중
protected:
	virtual void SetbIsSlowMotion(bool slowmotion) override;

protected:
	uint8 bIsSlow : 1;

	//패리 이펙트  - 포스트 프로세스 볼륨
protected:
	virtual void OnParryPostPorcess(bool effect) override;



	//대시 , 회피

public:
	FOnDashDelegate OndashDelegate;
protected:
	void OnDash();

	void OnAvoidEffect();

	void PlayEvadeMontage();

	void PlayEvadeCameraShake();

	void  OnEvadePostPorcess(bool effect);

	void EndEvadeMontage(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	
	void SetEvadeRotation(FVector TargetVector);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	uint8 bIsDash : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashTime=0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashColltime = 0.8f;

	FVector DashVector;

	FTimerHandle DashTimerHandle;

	FTimerHandle DashCooltimeTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float OriginalMaxWalkSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float OriginalMaxAcceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	uint8 bIsEvade : 1;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	TObjectPtr<class UAnimMontage> LeftEvadeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	TObjectPtr<class UAnimMontage> RightEvadeMontage;*/

	static const FString LeftEvadeMontage;
	static const FString RightEvadeMontage;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<class UAnimMontage> CurrentEvadeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	TSubclassOf<class UCameraShakeBase>	EvadeCameraShakeClass;

	FTimerHandle EvadeTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float EvadeTime = 0.5f;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	TObjectPtr<class UAnimMontage> DashMontage;*/
	
	static const FString DashMontage;

	//범위 슬로우
protected:
	virtual void OnSlowOVerlapToNPC(float time) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow")
	float SlowRadius = 500.0f;


public:
	virtual bool CanPlayerTarget(APawn* enemy) override;

	virtual void SetPlayerTargetPawn(APawn* enemy) override;

	virtual void DeletePlayerTargetPawn(APawn* enemy)override;

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<APawn>> TargetMePawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxTargets = 3;


	//스킬 입력
protected:
	void InputSkill();

	virtual void SetbIsNikkeSkill(bool skill) override;

	//니케 한정
	void StopDefenceNikke();
	
	//궁극기
	//궁극기 컷신중 이동제한
protected:
	void OnUltimateSkill();

public:
	class ULevelSequence* GetLevelSequence();

	UFUNCTION()
	void ChangeViewTarget(bool bstart);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UltiSkill")
	TObjectPtr<class ULevelSequence> LevelSequence;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> CutSceneCamera;
	

	//캐릭터별 위젯보관
protected:
	void CreateHudWidget();

public:
	void HudWidgetAddviewport();

	void RemoveHudWidget();

protected:
	//HUD의 클래스 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UPGHudWidget> PGHudWidgetClass;

	//생성할 위젯을 담을 위젯 포인트 보관하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UPGHudWidget> PGHudWidget;
		

	//캐릭터 교체 
protected:
	void OneChangePlayerCharacter();
	void TwoChangePlayerCharacter();
	void ThreeChangePlayerCharacter();

	void CheckandChangePlayerCharacter(int8 num);

};
