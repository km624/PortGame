// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Sword.h"
#include "Character/PGBaseCharacter.h"
#include "Data/WeaponData.h"


	

void ASword::OnInitializeWeapon(APGBaseCharacter* BaseCharacter,UWeaponData* weaponData)
{
	Super::OnInitializeWeapon(BaseCharacter, weaponData);

	if (WeaponMesh)
	{
		WeaponStaticComponent->SetStaticMesh(WeaponMesh);

	}
	if (weaponData)
	{
		WeaponSocket = weaponData->WeaponSocket;
	}

	
}

void ASword::Attack()
{
	Super::Attack();
	
	ComboStart();

}
