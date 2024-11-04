// Fill out your copyright notice in the Description page of Project Settings.



#include "Weapon/Rifle.h"
#include "Character/PGBaseCharacter.h"
#include "Animation/AnimMontage.h"


void ARifle::WeaponAttachment(APGBaseCharacter* BaseCharacter)
{
	Super::WeaponAttachment(BaseCharacter);
	WeaponaSkeletalComponent->
		SetupAttachment(OwnerCharacter->GetMesh(), TEXT("weaponSowrdSocket"));
	WeaponaSkeletalComponent->SetSkeletalMesh(WeaponMesh);
	
	
	Currentammo = ammoMaxCount;
}

void ARifle::Attack()
{
	Super::Attack();

	if (OwnerCharacter->GetCurrentIsAiming())
		StartFire();
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

	const UWorld* currentWorld = GetWorld();
	DrawDebugLine(currentWorld, start, end, FColor::Red, false, 1.0f);
	if (currentWorld)
	{
		// ИэСп!
		if (currentWorld->LineTraceSingleByChannel(
			hitResult,
			start,
			end,
			ECC_Visibility,
			collisionParams))
		{
			if (hitResult.GetActor())
			{
				auto* hitActor = hitResult.GetActor();
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Hit Actor Name: %s"), *hitActor->GetActorLabel()));
			}

		}

	}

	UE_LOG(LogTemp, Warning, TEXT("%d"), Currentammo);


}
