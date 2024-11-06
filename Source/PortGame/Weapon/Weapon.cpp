// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Character/PGBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/ComboData.h"
#include "Animation/AnimMontage.h"
#include "PortGame/PortGame.h"


AWeapon::AWeapon()
{
	WeaponaSkeletalComponent = 
		CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));

	SetRootComponent(WeaponaSkeletalComponent);
	
}

void AWeapon::OnInitializeWeapon(APGBaseCharacter* BaseCharacter)
{
	OwnerCharacter = Cast<APGBaseCharacter>(BaseCharacter);
	SLOG(TEXT("WeaponAttachment"));
}


void AWeapon::Attack()
{
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}





void AWeapon::ComboStart()
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

void AWeapon::ComboBegin()
{
	// Combo Status
	CurrentCombo = 1;

	// Movement Setting
	//None �����ϸ� �̵���� ���ּ� �̵� ����
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	// Animation Setting
	//const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboMontage, 1.0f);

	//��Ÿ�� ����ɶ�  ComboActiosnEnd �Լ� ȣ�� �ǰ� ��������Ʈ ȣ��
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AWeapon::ComboEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboMontage);

	ComboTimerHandle.Invalidate();
	ComboTimerdelayHandle.Invalidate();
	ComboCheckTimer();
}

void AWeapon::ComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));
	//UE_LOG(LogTemp, Warning, TEXT("ComboCheckTimer"));
	//���� ���ǵ嵵 ���ݿ���
   //const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
   //�ߵ��� �ð� ������ ������� ����
	float ComboEffectiveTime = (ComboData->EffectiveFrameCount[ComboIndex] / ComboData->FrameRate);
	if (ComboEffectiveTime > 0.0f)
	{
		//Ÿ�̸� �ߵ�
		// ComboCheck �Լ� ����
		//���� �������� �ǹ� �ݺ����� �ʵ��� -  False
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AWeapon::ComboCheck, ComboEffectiveTime, false);

		//�����̸� ���� Ÿ�̸� ,���� Ÿ�̸�
		/*GetWorld()->GetTimerManager().SetTimer(ComboTimerdelayHandle,
			FTimerDelegate::CreateLambda([this]() {ComboOK = true; }), ComoboDelay, false);*/
	}


}

void AWeapon::ComboCheck()
{
	//UE_LOG(LogTemp, Warning, TEXT("ComboCheck"));
	ComboTimerHandle.Invalidate();
	ComboTimerdelayHandle.Invalidate();

	if (HasNextComboCommand)
	{

		//UE_LOG(LogTemp, Warning, TEXT("ComboStart"));
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

		FOnMontageEnded* CurrentMontage = AnimInstance->Montage_GetEndedDelegate(ComboMontage);
		CurrentMontage->Unbind();
		//maxComboCount �޺� ���� ����� �ȵǱ� ������ 
		//���� �޺� ���� ���� 
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboData->MaxComboCount);

		//���� ������ �̸� ���� ����
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboData->MontageSectionName, CurrentCombo);


		//��Ÿ�� ���� ������ ����
		AnimInstance->Montage_SetNextSection(AnimInstance->Montage_GetCurrentSection(), NextSection, ComboMontage);



		//��Ÿ�� ���� ���� ���
		AnimInstance->Montage_Play(ComboMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(NextSection, ComboMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AWeapon::ComboEnd);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboMontage);

		if (CurrentCombo == ComboData->MaxComboCount)
			return;


		ComboCheckTimer();

		HasNextComboCommand = false;
	}


}

void AWeapon::ComboEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	//UE_LOG(LogTemp, Warning, TEXT("END"));

	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	//ĳ���� �����Ʈ �̵� ���Ѱ� �ٽ� ����
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	//12�� AI  - AI�� �������� �ľ��� �� �ְ� �߰�
	//NotifyComboActionEnd();
}




