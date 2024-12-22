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
                // ĳ���� Ȱ��ȭ
               
				Character->SetupCharacterData(SpawnParams.CharacterData);
                Character->SetteamId(SpawnParams.TeamId);
				Character->ChangeNpcColor();
				Character->SetActorLocationAndRotation(SpawnParams.SpawnLocation,SpawnParams.SpawnRotation);
                Character->EnableCharacter();

                // ��Ʈ�ѷ� ���� �ʱ�ȭ
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

		//���ǽ� �ٷ� �ൿƮ�� ����
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
		// ĳ���� ��Ȱ��ȭ 
		// //�ѹ���
		Character->SetActorEnableCollision(false);
		Character->SetActorHiddenInGame(true);
		Character->SetActorTickEnabled(false);

		// ��Ʈ�ѷ� �ʱ�ȭ
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

	// Ű ���� ���� Ȯ��
	if (!WeaponObjectPool.Contains(weaponclass))
	{
		SLOG(TEXT("WeaponObjectPool does not contain the key: %s"), *weaponclass.Get()->GetName());
		return nullptr;
	}

	TArray<AWeapon*>& WeaponArray = WeaponObjectPool[weaponclass];

	// ��� ������ ���� �˻�
	for (AWeapon* Weapon : WeaponArray)
	{
		if (Weapon && !Weapon->GetActorEnableCollision())
		{
			// ���� ��Ȱ��ȭ
			Weapon->SetActorTickEnabled(true);
			Weapon->SetActorHiddenInGame(false);
			Weapon->SetActorEnableCollision(true);
			//SLOG(TEXT("Recycled weapon: %s"), *Weapon->GetActorNameOrLabel());
			return Weapon;
		}
	}

	// �� ���� ����
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

	// ���⸦ ��Ȱ��ȭ
	weapon->SetActorTickEnabled(false);
	weapon->SetActorHiddenInGame(true);
	weapon->SetActorEnableCollision(false);
	
	//SLOG(TEXT("returnweapon : %s"), *weapon->GetActorNameOrLabel())

}
