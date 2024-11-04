// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Sword.h"
#include "Character/PGBaseCharacter.h"

void ASword::WeaponAttachment(APGBaseCharacter* BaseCharacter)
{
	Super::WeaponAttachment(BaseCharacter);
	WeaponaSkeletalComponent->
		SetupAttachment(OwnerCharacter->GetMesh(), TEXT("weaponSowrdSocket"));
	WeaponaSkeletalComponent->SetSkeletalMesh(WeaponMesh);
}

void ASword::Attack()
{
	Super::Attack();
	
	ComboStart();

}
