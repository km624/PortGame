// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Struct/PGGunStat.h"
#include "Rifle.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32 /*CurrentAmmo*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnbIsGunReload, bool /*bIsGunReload*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnbInGunRanged, bool /*bIsGunRange*/);
UCLASS()
class PORTGAME_API ARifle : public AWeapon
{
	GENERATED_BODY()


public:
	ARifle();

	virtual void Tick(float DeltaTime) override;

	virtual void OnInitializeWeapon(class  APGBaseCharacter* BaseCharacter,class UWeaponData* weaponData) override;

	virtual void Attack() override;

	

protected:
	void SetUpGunStat();

	void InGunRange(bool bisaim);

	UFUNCTION()
	void ShootCheck(bool bIsShoot);

	void StartFire();

	void StopFire();
	
	UFUNCTION()
	void StartReloading();

	void EndReloading();


	void FireWithLineTrace();

public:
	FOnAmmoChanged OnAmmoChanged;

	FOnbIsGunReload OnbIsGunReload;

	FOnbInGunRanged OnbInGunRanged;

	FORCEINLINE int32 GetammoMaxCount() { return ammoMaxCount; }

	FORCEINLINE float GetReloadMotagetime() {return ReloadMontageTime;}

		//ÃÑ ¹ß»ç ±¸Çö
protected:
	int32 ammoMaxCount;
	
	int32 Currentammo;

	float reloadingTime;

	float ShootInterval;

	float traceDistance;

	uint8 bIsRifle : 1;

	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;
	FTimerHandle StopTimerHandle;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float GunDamage;

	float ReloadMontageTime;

	bool bIsGunReloaded;

	uint8 bInGunRange : 1;

	FName FireLocation = TEXT("FireLocation");

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ReloadMontage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FPGGunStat GunStat;

	//ÃÑ±â¹Ýµ¿
protected:
	void StartGunEffect();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CamreShake)
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> GunNiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effect)
	TObjectPtr<class UNiagaraSystem> NAGunEffect;
	




	
}; 