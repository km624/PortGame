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
	//�ʱ� ����  - ĸ�� /�����Ʈ ������Ʈ
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	//���ӷ�
	//������ 2048
	GetCharacterMovement()->MaxAcceleration = 1500.0f;

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	//GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Mesh ��ġ�� ȸ�� ���� �ȸ¾Ƽ�
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

	//Combo ���� �־���
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

	//StatComponent �߰�
	Stat = CreateDefaultSubobject<UPGStatComponent>(TEXT("STAT"));


	
}

void APGBaseCharacter::ComboStart()
{
	if (CurrentCombo == 0)
	{
		
		ComboBegin();
		return;
	}

	
	//Ÿ�̸Ӱ� ������ �ȵ��������� �Է��� ���ưų�
	// �޺� �� ������
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
	//None �����ϸ� �̵���� ���ּ� �̵� ����
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	//const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboMontage, 1.0f);

	//��Ÿ�� ����ɶ�  ComboActiosnEnd �Լ� ȣ�� �ǰ� ��������Ʈ ȣ��
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
	 //���� ���ǵ嵵 ���ݿ���
	//const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	//�ߵ��� �ð� ������ ������� ����
	float ComboEffectiveTime = (ComboData->EffectiveFrameCount[ComboIndex] / ComboData->FrameRate);
	if (ComboEffectiveTime > 0.0f)
	{
		//Ÿ�̸� �ߵ�
		// ComboCheck �Լ� ����
		//���� �������� �ǹ� �ݺ����� �ʵ��� -  False
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &APGBaseCharacter::ComboCheck,  ComboEffectiveTime,false);

		//�����̸� ���� Ÿ�̸� ,���� Ÿ�̸�
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
		//maxComboCount �޺� ���� ����� �ȵǱ� ������ 
		//���� �޺� ���� ���� 
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboData->MaxComboCount);

		//���� ������ �̸� ���� ����
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboData->MontageSectionName, CurrentCombo);

		
		//��Ÿ�� ���� ������ ����
		AnimInstance->Montage_SetNextSection(AnimInstance->Montage_GetCurrentSection() , NextSection, ComboMontage);
			
		
		
		//��Ÿ�� ���� ���� ���
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
	//ĳ���� �����Ʈ �̵� ���Ѱ� �ٽ� ����
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	//12�� AI  - AI�� �������� �ľ��� �� �ְ� �߰�
	//NotifyComboActionEnd();
}



// �޺� ���� �ְ� �ϴ� ��Ƽ����
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



