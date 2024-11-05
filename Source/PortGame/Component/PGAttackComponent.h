// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PGAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTGAME_API UPGAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPGAttackComponent();

protected:

	virtual void InitializeComponent() override;
	
	virtual void BeginPlay() override;

	

public:
	void SetUpWeapon();

	void AttackToWeapon();


public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponClass")
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<class AWeapon> Weapon;

	


	

		
};
