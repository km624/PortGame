// Fill out your copyright notice in the Description page of Project Settings.



#include "Weapon/Rifle.h"
#include "Character/PGBaseCharacter.h"
#include "Animation/AnimMontage.h"
#include "PortGame/PortGame.h"
#include "Engine/DamageEvents.h"




ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OwnerCharacter->GetCurrentIsShooting())
		StartFire();
	else
		StopFire();
	

}

void ARifle::OnInitializeWeapon(APGBaseCharacter* BaseCharacter)
{
	Super::OnInitializeWeapon(BaseCharacter);
	
	if (IsValid(WeaponMesh))
	{
		WeaponaSkeletalComponent->SetSkeletalMesh(WeaponMesh);

	}
	
	WeaponSocket = TEXT("weaponRifleSocket");

	Currentammo = ammoMaxCount;

	
}

void ARifle::Attack()
{
	Super::Attack();

	if (OwnerCharacter->GetCurrentIsShooting())
		FireWithLineTrace();
	else
		ComboStart();
}



void ARifle::StartFire()
{
	if (!FireTimerHandle.IsValid() && !ReloadTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(
			FireTimerHandle,
			[this]() {FireWithLineTrace(); }, ShootInterval, true
		);
	}
}

void ARifle::StopFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop"));
	if (FireTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
	}

}

void ARifle::Reloading()
{
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ReloadMontage, 1.0f);
	GetWorld()->GetTimerManager().SetTimer(
		ReloadTimerHandle,
		[this]() {
			Currentammo = ammoMaxCount;
			UE_LOG(LogTemp, Warning, TEXT("Reloading"));
			ReloadTimerHandle.Invalidate();
		}, reloadingTime, false
	);
}

void ARifle::FireWithLineTrace()
{

	if (Currentammo <= 0)
	{
		StopFire();
		Reloading();
		UE_LOG(LogTemp, Warning, TEXT("NoArmo"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Time : %lf"), GetWorld()->GetTimeSeconds());
	Currentammo--;

	const FVector start = OwnerCharacter-> GetActorLocation();
	const FVector end = (OwnerCharacter->GetControlRotation().Vector() * traceDistance) + start;
	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);
	TArray <AActor*> ignoreActor;
	ignoreActor.Add(this);
	ignoreActor.Add(OwnerCharacter);
	collisionParams.AddIgnoredActors(ignoreActor);

	const UWorld* currentWorld = GetWorld();
	DrawDebugLine(currentWorld, start, end, FColor::Red, false, 1.0f);
	if (currentWorld)
	{
		bool OutHitResult = currentWorld->LineTraceSingleByChannel(
			hitResult,
			start,
			end,
			ECC_GameTraceChannel1,
			collisionParams);
		// ИэСп!
		if (OutHitResult)
		{

			FDamageEvent DamageEvent;
			hitResult.GetActor()->TakeDamage(10.0f, DamageEvent, OwnerCharacter->GetController(), this);
		}

	}

	UE_LOG(LogTemp, Warning, TEXT("%d"), Currentammo);


}
