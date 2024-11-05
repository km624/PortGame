// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API ARifle : public AWeapon
{
	GENERATED_BODY()


public:
	virtual void OnInitializeWeapon(class  APGBaseCharacter* BaseCharacter) override;

	virtual void Attack() override;

protected:

	void StartFire();

	void StopFire();

	void Reloading();

	void FireWithLineTrace();

		//ÃÑ ¹ß»ç ±¸Çö
protected:
	int32 ammoMaxCount = 10;
	
	int32 Currentammo;
	float reloadingTime = 3.0f;

	float ShootInterval = 1.0f;

	float traceDistance = 1000.0f;

	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	TObjectPtr<class UAnimMontage> ReloadMontage;
	
};
