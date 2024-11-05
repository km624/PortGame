// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Sword.h"
#include "Character/PGBaseCharacter.h"


	

void ASword::OnInitializeWeapon(APGBaseCharacter* BaseCharacter)
{
	Super::OnInitializeWeapon(BaseCharacter);

	if (WeaponMesh)
	{
		WeaponaSkeletalComponent->SetSkeletalMesh(WeaponMesh);

	}
	WeaponSocket =  TEXT("weaponSowrdSocket");
	
}

void ASword::Attack()
{
	Super::Attack();
	
	ComboStart();

}
