// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Sword.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API ASword : public AWeapon
{
	GENERATED_BODY()


public:
	virtual void OnInitializeWeapon(class  APGBaseCharacter* BaseCharacter) override;
	
	virtual void Attack() override;

	
	
};
