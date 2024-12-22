// Fill out your copyright notice in the Description page of Project Settings.


#include "Field/ObjectPoolManager.h"
#include "AI/PGAIController.h"
#include "Character/PGNpcCharacter.h"
#include "Data/BaseCharacterDataAsset.h"
#include "Field/PGField.h"
#include "PortGame/PortGame.h"
#include "Weapon/Weapon.h"
#include "Weapon/Rifle.h" 
#include "Weapon/Sword.h" 

UObjectPoolManager::UObjectPoolManager()
{
}

void UObjectPoolManager::InitializePool()
{
	TArray<AWeapon*> RifleArray;
	WeaponObjectPool.Add(ARifle::StaticClass(), RifleArray);

	TArray<AWeapon*> SwordArray;
	WeaponObjectPool.Add(ASword::StaticClass(), SwordArray);

}

APGNpcCharacter* UObjectPoolManager::GetPooledObject(const FCharacterSpawnParams& SpawnParams)
{
    if (ObjectPool.Num() > 0)
    {
        for (auto& character : ObjectPool)
        {
            APGNpcCharacter* Character = character.Key;
            if (Character && !Character->IsActorTickEnabled())
            {
                // 캐릭터 활성화
               
				Character->SetupCharacterData(SpawnParams.CharacterData);
                Character->SetteamId(SpawnParams.TeamId);
				Character->ChangeNpcColor();
				Character->SetActorLocationAndRotation(SpawnParams.SpawnLocation,SpawnParams.SpawnRotation);
                Character->EnableCharacter();

                // 컨트롤러 상태 초기화
                APGAIController* Controller = character.Value;
                if (Controller)
                {
					if (SpawnParams.bFieldProtect)
						Controller->SetMyFieldData(SpawnParams.Field);
					else
						Controller->SetAttackAIData();

                    Controller->Possess(Character);

					//SLOG(TEXT("Recycle : %s"), *Character->GetActorNameOrLabel())
                    return Character;
                }
            }
        }
    }

	APGNpcCharacter* AiCharacter = Cast<APGNpcCharacter>((SpawnParams.Field->GetWorld()->SpawnActorDeferred<APGNpcCharacter>(
		APGNpcCharacter::StaticClass(),
		FTransform(SpawnParams.SpawnRotation, SpawnParams.SpawnLocation),
		SpawnParams.Field,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	)));

	if (AiCharacter)
	{
		AiCharacter->SetCharacterData(SpawnParams.CharacterData);
		AiCharacter->SetteamId(SpawnParams.TeamId);

		AiCharacter->FinishSpawning(FTransform(SpawnParams.SpawnRotation, SpawnParams.SpawnLocation));
	}

	APGAIController* pgAIcontoller = Cast<APGAIController>((GetWorld()->SpawnActorDeferred<APGAIController>(
		APGAIController::StaticClass(),
		FTransform(SpawnParams.SpawnRotation, SpawnParams.SpawnLocation),
		SpawnParams.Field,
		nullptr
	)));

	if (pgAIcontoller)
	{
		pgAIcontoller->FinishSpawning(FTransform(SpawnParams.SpawnRotation, SpawnParams.SpawnLocation));
		if (SpawnParams.bFieldProtect)
			pgAIcontoller->SetMyFieldData(SpawnParams.Field);
		else
			pgAIcontoller->SetAttackAIData();

		//빙의시 바로 행동트리 시작
		pgAIcontoller->Possess(AiCharacter);
	}

	if (AiCharacter && pgAIcontoller)
	{
		ObjectPool.Add(AiCharacter, pgAIcontoller);

		//SLOG(TEXT("NewCharacter : %s"), *AiCharacter->GetActorNameOrLabel())
		return AiCharacter;
	}

	return nullptr;
}

void UObjectPoolManager::ReturnObjectToPool(APGNpcCharacter* Character)
{
	if (Character && ObjectPool.Contains(Character))
	{
		// 캐릭터 비활성화 
		// //한번더
		Character->SetActorEnableCollision(false);
		Character->SetActorHiddenInGame(true);
		Character->SetActorTickEnabled(false);

		// 컨트롤러 초기화
		APGAIController* Controller = ObjectPool[Character];
		if (Controller)
		{
			/*Controller->UnPossess();
			Controller->ResetController();*/
		}

		//SLOG(TEXT("return : %s"), *Character->GetActorNameOrLabel())
	}

}

AWeapon* UObjectPoolManager::GetWeaponPooledObject(TSubclassOf<AWeapon> weaponclass)
{
	if (!weaponclass)
	{
		SLOG(TEXT("Invalid weapon class passed to GetWeaponPooledObject."));
		return nullptr;
	}

	// 키 존재 여부 확인
	if (!WeaponObjectPool.Contains(weaponclass))
	{
		SLOG(TEXT("WeaponObjectPool does not contain the key: %s"), *weaponclass.Get()->GetName());
		return nullptr;
	}

	TArray<AWeapon*>& WeaponArray = WeaponObjectPool[weaponclass];

	// 사용 가능한 무기 검색
	for (AWeapon* Weapon : WeaponArray)
	{
		if (Weapon && !Weapon->GetActorEnableCollision())
		{
			// 무기 재활성화
			Weapon->SetActorTickEnabled(true);
			Weapon->SetActorHiddenInGame(false);
			Weapon->SetActorEnableCollision(true);
			//SLOG(TEXT("Recycled weapon: %s"), *Weapon->GetActorNameOrLabel());
			return Weapon;
		}
	}

	// 새 무기 생성
	AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(weaponclass);
	if (SpawnedWeapon)
	{
		WeaponArray.Add(SpawnedWeapon);
		//SLOG(TEXT("Spawned new weapon: %s"), *SpawnedWeapon->GetActorNameOrLabel());
		return SpawnedWeapon;
	}

	SLOG(TEXT("Failed to spawn weapon of class: %s"), *weaponclass->GetName());
	return nullptr;
}


void UObjectPoolManager::ReturnWeaponObjectToPool(AWeapon* weapon)
{
	if (!weapon) return;

	// 무기를 비활성화
	weapon->SetActorTickEnabled(false);
	weapon->SetActorHiddenInGame(true);
	weapon->SetActorEnableCollision(false);
	
	//SLOG(TEXT("returnweapon : %s"), *weapon->GetActorNameOrLabel())

}
