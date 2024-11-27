// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Character/PGBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/ComboData.h"
#include "Animation/AnimMontage.h"
#include "PortGame/PortGame.h"
#include "Data/WeaponData.h"
#include "Character/PGPlayerCharacter.h"

AWeapon::AWeapon()
{
	WeaponStaticComponent = 
		CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));

	SetRootComponent(WeaponStaticComponent);
	WeaponStaticComponent->SetCollisionProfileName(TEXT("WeaponColli"));
	
}

void AWeapon::OnInitializeWeapon(APGBaseCharacter* BaseCharacter,UWeaponData* weaponData)
{
	OwnerCharacter = Cast<APGBaseCharacter>(BaseCharacter);
	if (weaponData)
	{
		WeaponMesh = weaponData->WeaponMesh;
		ModifierStat = weaponData->ModifierStat;
		ComboMontage = weaponData->ComboMontage;
		ComboData = weaponData->ComboData;
	}

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

void AWeapon::ComboBegin()
{
	// Combo Status
	CurrentCombo = 1;


	
	// Animation Setting
	//const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	ComboPlayTime = OwnerCharacter->GetTotalStat().AttackSpeed;

	AnimInstance->Montage_Play(ComboMontage, ComboPlayTime);

	//플레이어 캐릭터만 값 받아서 돌리기
	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(OwnerCharacter);
	if (playerCharacter)
	{
		playerCharacter->SetbIsAttackRotation(true);
	}
	//몽타주 종료될때  ComboActiosnEnd 함수 호출 되게 델리게이트 호출
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
	//어택 스피드도 스텟에서
   //const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
   //발동할 시간 정보를 얻기위한 변수
	float ComboEffectiveTime = (ComboData->EffectiveFrameCount[ComboIndex] / ComboData->FrameRate) /ComboPlayTime;
	if (ComboEffectiveTime > 0.0f)
	{
		//타이머 발동
		// ComboCheck 함수 실행
		//가록 마지막의 의미 반복하지 않도록 -  False
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AWeapon::ComboCheck, ComboEffectiveTime, false);

		//딜레이를 위한 타이머 ,깡통 타이머
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
		//maxComboCount 콤보 값을 벗어나면 안되기 때문에 
		//다음 콤보 숫자 저장 
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboData->MaxComboCount);

		//다음 섹션의 이름 정보 저장
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboData->MontageSectionName, CurrentCombo);


		//몽타주 다음 섹션을 연결
		AnimInstance->Montage_SetNextSection(AnimInstance->Montage_GetCurrentSection(), NextSection, ComboMontage);



		//몽타주 다음 섹션 재생
		AnimInstance->Montage_Play(ComboMontage, ComboPlayTime);
		AnimInstance->Montage_JumpToSection(NextSection, ComboMontage);

		//플레이어 캐릭터만 값 받아서 돌리기
		APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(OwnerCharacter);
		if (playerCharacter)
		{
			playerCharacter->SetbIsAttackRotation(true);
		}

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
	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(OwnerCharacter);
	if (playerCharacter)
	{
		playerCharacter->SetbIsAttackRotation(false);
	}
	
	//12강 AI  - AI가 끝날때를 파악할 수 있게 추가
	//NotifyComboActionEnd();
}




