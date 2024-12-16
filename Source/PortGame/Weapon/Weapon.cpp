// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Character/PGBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/ComboData.h"
#include "Animation/AnimMontage.h"
#include "PortGame/PortGame.h"
#include "Data/WeaponData.h"
#include "Character/PGPlayerCharacter.h"
#include "Interface/PGAICharacterInterface.h"
#include "Data/CharacterEnumData.h"


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
		ComboMontage = weaponData->GetComboMontageAsString();
		ComboData = weaponData->ComboData;
		NAWeaponEffect = weaponData->NAWeaponEffect;

		OwnerCharacter->LoadAndPlayMontageByPath(OwnerCharacter->CharacterName, ComboMontage);
	}

	//SLOG(TEXT("WeaponAttachment"));
}


void AWeapon::Attack()
{
	
}



void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}


void AWeapon::SetHasNextCombo(bool nextcombo)
{
	HasNextComboCommand = nextcombo;
	if (OwnerCharacter->GetPlayerCharacterType() == EPlayerCharacterType::Nikke) return;

	if (HasNextComboCommand)
	{
		ComboCheck();
	}
}

void AWeapon::ComboStart()
{
	if (CurrentCombo == 0)
	{

		ComboBegin();
		
		return;
	}

}

void AWeapon::ComboBegin()
{
	
	CurrentCombo = 1;
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	ComboPlayTime = OwnerCharacter->GetTotalStat().AttackSpeed;
	
	//Ai면 공격 속도 낮추기
	if (OwnerCharacter->ActorHasTag(TAG_AI))
		ComboPlayTime *= 0.8f;
	
	AnimInstance->Montage_Play(OwnerCharacter->AllMontage[ComboMontage], ComboPlayTime);
	
	//플레이어 캐릭터만 값 받아서 돌리기
	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(OwnerCharacter);
	if (playerCharacter)
	{
		playerCharacter->SetbIsAttackRotation(true);
	}
	
	EndDelegate.BindUObject(this, &AWeapon::ComboEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, OwnerCharacter->AllMontage[ComboMontage]);
	CurrentMontageEndDelegate = AnimInstance->Montage_GetEndedDelegate(OwnerCharacter->AllMontage[ComboMontage]);
	
}


void AWeapon::ComboCheck()
{
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (HasNextComboCommand)
	{

		if (!CurrentMontageEndDelegate)
		{
			SLOG(TEXT("WHartthe"));
			return;
		}
		CurrentMontageEndDelegate->Unbind();
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboData->MaxComboCount);

		//다음 섹션의 이름 정보 저장
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboData->MontageSectionName, CurrentCombo);


		//몽타주 다음 섹션을 연결
		AnimInstance->Montage_SetNextSection(AnimInstance->Montage_GetCurrentSection(), NextSection, OwnerCharacter->AllMontage[ComboMontage]);

		//SLOG(TEXT("NextSection : %s"), *NextSection.ToString());
		
		//몽타주 다음 섹션 재생
		AnimInstance->Montage_Play(OwnerCharacter->AllMontage[ComboMontage], ComboPlayTime);
		AnimInstance->Montage_JumpToSection(NextSection, OwnerCharacter->AllMontage[ComboMontage]);


		//플레이어 캐릭터만 값 받아서 돌리기
		APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(OwnerCharacter);
		if (playerCharacter)
		{
			playerCharacter->SetbIsAttackRotation(true);
		}

		//FOnMontageEnded EndDelegate;
		 EndDelegate.BindUObject(this, &AWeapon::ComboEnd);
		 AnimInstance->Montage_SetEndDelegate(EndDelegate, OwnerCharacter->AllMontage[ComboMontage]);
		 CurrentMontageEndDelegate = AnimInstance->Montage_GetEndedDelegate(OwnerCharacter->AllMontage[ComboMontage]);

		
		if (CurrentCombo == ComboData->MaxComboCount)
		{

			return;
		}
			

		HasNextComboCommand = false;

		
	}
	

}

void AWeapon::ComboEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{

	
	//ensure(CurrentCombo != 0);
	
	CurrentCombo = 0;
	APGPlayerCharacter* playerCharacter = Cast<APGPlayerCharacter>(OwnerCharacter);
	if (playerCharacter)
	{
		playerCharacter->SetbIsAttackRotation(false);
	}
	
	//12강 AI  - AI가 끝날때를 파악할 수 있게 추가
	NotifyComboActionEnd();
}

void AWeapon::NotifyComboActionEnd()
{
	IPGAICharacterInterface* NPCEnd = Cast<IPGAICharacterInterface>(OwnerCharacter);
	if (NPCEnd)
	{
		NPCEnd->NotifyComboEnd();
	}
}




