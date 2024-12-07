// Fill out your copyright notice in the Description page of Project Settings.



#include "Weapon/Rifle.h"
#include "Character/PGBaseCharacter.h"
#include "Animation/AnimMontage.h"
#include "PortGame/PortGame.h"
#include "Engine/DamageEvents.h"
#include "Weapon/BlockAim.h"
#include "Data/WeaponData.h"
#include "Data/GunWeaponData.h"
#include "Interface/PlayerCameraShakeInterface.h"
#include "Physics/PGCollision.h"
#include "Interface/AttackHitStopInterface.h"
#include "Character/PGNpcCharacter.h"

ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARifle::OnInitializeWeapon(APGBaseCharacter* BaseCharacter, UWeaponData* weaponData)
{
	Super::OnInitializeWeapon(BaseCharacter, weaponData);
	
	if (IsValid(WeaponMesh))
	{
		WeaponStaticComponent->SetStaticMesh(WeaponMesh);

	}
	
	OwnerCharacter->OnbIsShoot.AddUObject(this,
		&ThisClass::ShootCheck);
	OwnerCharacter->OnbIsReload.AddUObject(this,
		&ThisClass::Reloading);

	if (weaponData)
	{
		UGunWeaponData* gunWeaponData = Cast<UGunWeaponData>(weaponData);
		WeaponSocket = gunWeaponData->WeaponSocket;
		GunStat = gunWeaponData->GunStat;
		ReloadMontage = gunWeaponData->ReloadMontage;
		CameraShakeClass = gunWeaponData->CameraShakeClass;
		
		SetUpGunStat();
	}
	Currentammo = ammoMaxCount;

}

void ARifle::Attack()
{
	Super::Attack();
	if (OwnerCharacter->GetCurrentIsShooting())
	{
		
		if (!FireTimerHandle.IsValid() && !ReloadTimerHandle.IsValid() && !StopTimerHandle.IsValid()
			&& CurrentCombo == 0)
		{
			FireWithLineTrace();
			StartFire();
		}
			
	}
	else
	{
		
		if (ReloadTimerHandle.IsValid())
		{
			GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
			UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
			
			AnimInstance->StopAllMontages(0.0f);
			OwnerCharacter->SetbIsReload(false);
		}
		ComboStart();
		
	}
		
}



void ARifle::SetUpGunStat()
{
	ammoMaxCount = GunStat.ammoMaxCount;
	reloadingTime = GunStat.reloadingTime;
	ShootInterval = GunStat.ShootInterval;
	traceDistance = GunStat.traceDistance;
}

void ARifle::ShootCheck(bool bIsShoot)
{
	if (bIsShoot)
		Attack();
	else
		StopFire();

}

void ARifle::StartFire()
{
	if (!FireTimerHandle.IsValid() && !ReloadTimerHandle.IsValid()
		&& CurrentCombo == 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
			FireTimerHandle,
			[this]() {FireWithLineTrace(); }, ShootInterval, true
		);
	}
}

void ARifle::StopFire()
{
	if (StopTimerHandle.IsValid())return;
	//UE_LOG(LogTemp, Warning, TEXT("Stop"));
	if (FireTimerHandle.IsValid())
	{

		GetWorldTimerManager().ClearTimer(FireTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(
			StopTimerHandle,
			[this]() {
				GetWorldTimerManager().ClearTimer(StopTimerHandle);
			}, 
			ShootInterval, false
		);
	}

}

void ARifle::Reloading()
{
	if (ReloadTimerHandle.IsValid())return;
	SLOG(TEXT("RELOADING"));
	OwnerCharacter->SetbIsReload(true);
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ReloadMontage, 1.0f);
	GetWorld()->GetTimerManager().SetTimer(
		ReloadTimerHandle,
		[this]() {
			Currentammo = ammoMaxCount;
			
			OwnerCharacter->SetbIsReload(false);
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
		//UE_LOG(LogTemp, Warning, TEXT("NoArmo"));
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Time : %lf"), GetWorld()->GetTimeSeconds());
	Currentammo--;
	

	const FVector Camerastart = OwnerCharacter->GetAimLocation();
	//카메라 시작지점
	FVector CameraLoc = OwnerCharacter->GetAimLocation();
	
	const FVector CameraEnd = CameraLoc +OwnerCharacter->GetController()->GetControlRotation().Vector() * (traceDistance);
	
	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	TArray <AActor*> ignoreActor;
	ignoreActor.Add(this);
	ignoreActor.Add(OwnerCharacter);
	collisionParams.AddIgnoredActors(ignoreActor);

	const UWorld* currentWorld = GetWorld();

	FVector end;
	DrawDebugLine(currentWorld, Camerastart, CameraEnd, FColor::Green, false, 1.0f);
	
	//플레이어인지 확인
	IAttackHitStopInterface* Player = Cast<IAttackHitStopInterface>(OwnerCharacter);
	if (Player)
	{
		if (currentWorld)
		{
			bool OutHitResult = currentWorld->LineTraceSingleByChannel(
				hitResult,
				Camerastart,
				CameraEnd,
				ECollisionChannel::ECC_Visibility,
				collisionParams);
			
			if (OutHitResult)
			{

				FVector ForwardVector = OwnerCharacter->GetController()->GetControlRotation().Vector();
				FVector HitLocationWithOffset = hitResult.Location + (ForwardVector * 50.0f);
				end = HitLocationWithOffset;

			
			}
			else
				end = OwnerCharacter->GetController()->GetControlRotation().Vector() * traceDistance;
			

		}
	}	
	else
	{
		//이때는 NPC
		APGNpcCharacter* NPCPlayer = Cast<APGNpcCharacter>(OwnerCharacter);
		if (NPCPlayer)
		{
			end = NPCPlayer->GetTargetPawn()->GetActorLocation();
		}
	}

	const FVector start = WeaponStaticComponent->GetSocketLocation(WeaponSocket);

	//데미지
	float GundDamage = OwnerCharacter->GetTotalStat().Attack*0.1f;
	if (OwnerCharacter->ActorHasTag(TAG_AI))
		GundDamage *= 0.5f;
	
	DrawDebugLine(currentWorld, start, end, FColor::Red, false, 1.0f);
	if (currentWorld)
	{
		bool OutHitResult = currentWorld->LineTraceSingleByChannel(
			hitResult,
			start,
			end,
			CCHANNEL_PGACTION,
			collisionParams);
		
		if (OutHitResult)
		{

			FDamageEvent DamageEvent;
			hitResult.GetActor()->TakeDamage(GundDamage, DamageEvent, OwnerCharacter->GetController(), OwnerCharacter);
		}

	}
	StartGaunRecoil();
	
	//UE_LOG(LogTemp, Warning, TEXT("%d"), Currentammo);


}

void ARifle::StartGaunRecoil()
{
	IPlayerCameraShakeInterface* gunrecoil = Cast<IPlayerCameraShakeInterface>(OwnerCharacter->GetController());
	if (gunrecoil)
	{
		gunrecoil->PlayCameraShake(CameraShakeClass);
	}
}




