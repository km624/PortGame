// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PGAIBaseCharacter.h"
//#include "Character/PGBaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "Interface/PGHudWidgetInterface.h"
#include "Interface/AttackHitStopInterface.h"
#include "Interface/AIBodyGuardInterface.h"
#include "Interface/AITargetPlayerInterface.h"
#include "Interface/PlayerAddEXPInterface.h"
#include "Interface/SetPlayerExecutionInterface.h"
#include "Interface/ExecutionEliteNPCInterface.h"
#include "PGPlayerCharacter.generated.h"
/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnDashDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnKOCountChangeDelegate, int32 /*KOCount*/);
UENUM(BlueprintType)
enum class ECameraMoveType :uint8
{
	AimCamera = 0 UMETA(DisplayName = "Aim"),
	AttackCamera UMETA(DisplayName = "Attack"),
	DashCamera UMETA(DisplayName = "Dash"),
	BodyGuardCamera UMETA(DisplayName = "BodyGuard"),
	FieldChangeCamera UMETA(DisplayName = "FieldChange"),
	ArmorBreakCamera UMETA(DisplayName = "ArmorBreak")
};
UENUM()
enum class EControlData : uint8
{
	Base,
	Aim
};
UCLASS(Blueprintable)
class PORTGAME_API UTimeLineWrapper : public UObject
{
	GENERATED_BODY()

public:
	// 타임라인 객체
	FTimeline Timeline;
};
UCLASS()
class PORTGAME_API APGPlayerCharacter : public APGAIBaseCharacter, public IPGHudWidgetInterface ,public IAttackHitStopInterface,  public IPlayerAddEXPInterface
	,public IAIBodyGuardInterface,public ISetPlayerExecutionInterface
{
	GENERATED_BODY()

public:

	APGPlayerCharacter();

	

protected:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetupCharacterData(class UBaseCharacterDataAsset* characterdata) override;

	void SetUpBodyGuardOption(TArray<class UBGBaseOptionDataAsset*>& optionDataAssets);

public:
	void SetCharacterInputData(EControlData DataName);


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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction>MapAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> BodyGuardOptionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ExecutionAction;

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

	//FTimeline AimTimeline;

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
	float DashTime = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashColltime = 1.0f;

	FVector DashVector;

	FTimerHandle DashTimerHandle;

	FTimerHandle DashCooltimeTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float OriginalMaxWalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float OriginalMaxAcceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	uint8 bIsEvade : 1;

	static const FString LeftEvadeMontage;
	static const FString RightEvadeMontage;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<class UAnimMontage> CurrentEvadeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	TSubclassOf<class UCameraShakeBase>	EvadeCameraShakeClass;

	FTimerHandle EvadeTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float EvadeTime = 0.5f;
	
	static const FString DashMontage;

	//범위 슬로우
protected:
	virtual void OnSlowOVerlapToNPC(float time,AActor* ignoreActor) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow")
	float SlowRadius = 500.0f;

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

	FORCEINLINE class UPGHudWidget* GetPlayerHudWidget() { return PGHudWidget; }
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

	//캐릭터 교체 ui
public:
	void SetupAllCharacterWidget(int32 num);

	void SetupMyCharacterWidgetToAnother(const TArray<class APGPlayerCharacter*>& allcharacters, float cooltime);

	void OnStartChangeCharacterWidget(int32 num);
	
	//미니맵 ui
public:
	void SetupPlayerMiniMap(const TArray<class APGPlayerCharacter*>& allcharacters, const TArray<class APGAIController*>& allaicontrollers);

	FORCEINLINE bool GetbIsMiniMap(){ return bIsMiniMap; }
protected:
	void ChangeMiniMapSize();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	uint8 bIsMiniMap : 1;


	//레벨 관련
public:

	void SetupPlayerLevel(int32 level);

	int32 GetPlayerCharacterLevel();

protected:

	virtual void PlayerAddEXP() override;

	void PlayLevelUpEffet(int32 currentlevel);

	//적 처치 카운트
protected:
	int32 KOCount = 0;

	FOnKOCountChangeDelegate FKoCountChanged;

	//게임 승리 , 패배 표시
public:
	void UpdateGameState(bool bIsclear);

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	uint8 bIsGameStated : 1;

	// 액터를 해집고 가기 위해 ( 임시)
protected:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor*OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	//공격 타격감을 위한 슬로우 ( npc의 패리 스테이트와 공통)
protected:
	virtual void AttackSlowStart() override;

	virtual void AttackSlowEnd() override;

	
	//공격 , 대쉬 등 공통 카메라 무빙

protected:
	void AllTimelineSetting();

	void AllTimelineTick(float dt);

	void AllTimelineStop(ECameraMoveType cameramovetype);

	UFUNCTION()
	void AttackCameraMove(float dt);

	UFUNCTION()
	void DashCameraMove(float dt);

	UFUNCTION()
	void BodyGuardCameraMove(float dt);

	UFUNCTION()
	void FieldChangeCameraMove(float dt);

	UFUNCTION()
	void ArmorBreakCameraMove(float dt);

	UFUNCTION()
	void OnArmorBreakTimelineFinished();

public:
	void StartSetCameraMoveSetting(bool bisreversed, ECameraMoveType cameramovetype);

protected:
	UPROPERTY()
	uint8 bIsReversed : 1;

	FVector CameraCurrentLocation;
	FRotator CameraCurrentRotator;
	
	UPROPERTY()
	TMap<ECameraMoveType, UTimeLineWrapper*> AllCameraTimeline;

	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TObjectPtr<class UCurveFloat> AttackCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TObjectPtr<class UCurveFloat> DashCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TObjectPtr<class UCurveFloat> BodyGuardCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TObjectPtr<class UCurveFloat> FieldChangeCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TObjectPtr<class UCurveFloat> ArmorBreakCurve;
public:
	//공격 카메라 타임라인
	//FTimeline AttackTimeline;

protected:
	//대쉬 카메라 타임라인
	//FTimeline DashTimeline;


	//바디가드 옵션
public:
	virtual bool CanPlayerProtect(APawn* pawn) override;

	virtual AActor* SetPlayerProtectPawn(APawn* pawn) override;

	//virtual FVector CalculateOffsetPawn(APawn* pawn)  override;

	virtual void BodyGuardOptionsClick(int32 optionnum, uint8 optionGauge) override;

	virtual void BoyGuardOptionHover(int32 optionnum) override;

	virtual void HoverUp()override;
		
	UFUNCTION()
	virtual void DeletePlayerProtectPawn(APawn* pawn);

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<class UAIBodyGuardComponent> AIBodyGuardComponent;

protected:
	void ShowBodyGuardOption();

	void CloseBodyGuardOption();

protected:
	uint8 bShowBodyGuardOption : 1;

	//FTimeline BodyGuardTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UBGBaseOptionDataAsset*> OptionDataAssets;

protected:
	void SetPostProcessMaterial();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Materials")
	TObjectPtr<class UMaterialInterface> PostProcessMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "Materials")
	TObjectPtr<class UMaterialInstanceDynamic> DynamicMaterialInstance;

public:
	void StartFieldChangedCamera(bool start);

protected:
	UPROPERTY()
	uint8 bIsGlobalTimeSlow : 1;
	

	//처형

protected:
	virtual void ArmorBreakCameraFocus(AActor* eliteNPC) override;

	virtual bool HasPlayerController() override;

	virtual void SetInExecutionRange(bool InRange, AActor* eliteNPC) override;
	
	void OnExecution();

	void StartExecution();

	void PlayExecutionMontage();

	void EndExecuitionMontage(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	
	void StartExecutionSequence();

	UFUNCTION()
	void FinishExecutionSequence();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bIsExecutionRange : 1;

	const static FString ExcuisonAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TScriptInterface<IExecutionEliteNPCInterface> ExecutionElite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bIsExecution : 1;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<AActor> ArmorBreakElite;

	FVector CameraWorldLocation;
	FRotator CameraWorldRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ALevelSequenceActor> ExecutionLevelSequenceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class ULevelSequence> ExecutionLevelSequence;


};
