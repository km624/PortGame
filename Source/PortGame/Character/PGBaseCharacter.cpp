// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/ComboData.h"
#include "Animation/AnimMontage.h"
#include "Component/PGStatComponent.h"



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

	//Combo 값들 넣어줌
	/*static ConstructorHelpers::FObjectFinder<UAnimMontage>comboMontage(TEXT("/Script/Engine.AnimMontage'/Game/PortGame/Animation/Combo/ComboMontage1.ComboMontage1'"));
	if (comboMontage.Object)
	{
		ComboMontage = comboMontage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UComboData>comboData(TEXT("/Script/PortGame.ComboData'/Game/PortGame/Animation/Combo/ComboData.ComboData'"));
	if (comboData.Object)
	{
		ComboData = comboData.Object;
	}*/

	//StatComponent 추가
	Stat = CreateDefaultSubobject<UPGStatComponent>(TEXT("STAT"));


	
}

void APGBaseCharacter::ComboStart()
{
	if (CurrentCombo == 0)
	{
		
		ComboBegin();
		return;
	}

	
	//타이머가 설정이 안되있을때는 입력을 놓쳤거나
	// 콤보 다 했을때
	if (ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = true;
		

	}
	else
	{
		
		HasNextComboCommand = false;
	}
}

void APGBaseCharacter::ComboBegin()
{
	// Combo Status
	CurrentCombo = 1;

	// Movement Setting
	//None 지정하면 이동기능 없애서 이동 멈춤
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	//const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboMontage, 1.0f);

	//몽타주 종료될때  ComboActiosnEnd 함수 호출 되게 델리게이트 호출
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APGBaseCharacter::ComboEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboMontage);

	ComboTimerHandle.Invalidate();
	ComboTimerdelayHandle.Invalidate();
	ComboCheckTimer();
}

void APGBaseCharacter::ComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));
	UE_LOG(LogTemp, Warning, TEXT("ComboCheckTimer"));
	 //어택 스피드도 스텟에서
	//const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	//발동할 시간 정보를 얻기위한 변수
	float ComboEffectiveTime = (ComboData->EffectiveFrameCount[ComboIndex] / ComboData->FrameRate);
	if (ComboEffectiveTime > 0.0f)
	{
		//타이머 발동
		// ComboCheck 함수 실행
		//가록 마지막의 의미 반복하지 않도록 -  False
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &APGBaseCharacter::ComboCheck,  ComboEffectiveTime,false);

		//딜레이를 위한 타이머 ,깡통 타이머
		/*GetWorld()->GetTimerManager().SetTimer(ComboTimerdelayHandle,
			FTimerDelegate::CreateLambda([this]() {ComboOK = true; }), ComoboDelay, false);*/
	}
	

}

void APGBaseCharacter::ComboCheck()
{
	//UE_LOG(LogTemp, Warning, TEXT("ComboCheck"));
	ComboTimerHandle.Invalidate();
	ComboTimerdelayHandle.Invalidate();

	if (HasNextComboCommand)
	{
		
		//UE_LOG(LogTemp, Warning, TEXT("ComboStart"));
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		FOnMontageEnded* CurrentMontage = AnimInstance->Montage_GetEndedDelegate(ComboMontage);
		CurrentMontage->Unbind();
		//maxComboCount 콤보 값을 벗어나면 안되기 때문에 
		//다음 콤보 숫자 저장 
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboData->MaxComboCount);

		//다음 섹션의 이름 정보 저장
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboData->MontageSectionName, CurrentCombo);

		
		//몽타주 다음 섹션을 연결
		AnimInstance->Montage_SetNextSection(AnimInstance->Montage_GetCurrentSection() , NextSection, ComboMontage);
			
		
		
		//몽타주 다음 섹션 재생
		AnimInstance->Montage_Play(ComboMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(NextSection, ComboMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &APGBaseCharacter::ComboEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboMontage);

		if (CurrentCombo == ComboData->MaxComboCount)
			return;
		
		
		ComboCheckTimer();

		HasNextComboCommand = false;
	}
	
		
}

void APGBaseCharacter::ComboEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	//UE_LOG(LogTemp, Warning, TEXT("END"));
	
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	//캐릭터 무브먼트 이동 못한거 다시 복구
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	//12강 AI  - AI가 끝날때를 파악할 수 있게 추가
	//NotifyComboActionEnd();
}



// 콤보 섹션 있게 하는 노티파이
//void APGBaseCharacter::ComboSectionEnd()
//{
//	
//	//if (HasNextComboCommand)
//	//{
//	//	ComboCheckTimer();
//	//	HasNextComboCommand = false;
//	//	//UE_LOG(LogTemp, Warning, TEXT("ComboSectionEnd_Go"));
//	//}
//	//	
//	//else
//	//{
//	//	//UE_LOG(LogTemp, Warning, TEXT("ComboSectionEnd_Stop"));
//	//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
//	//	AnimInstance->Montage_Stop(0.25f,ComboMontage);
//	//}
//}



