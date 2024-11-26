// Fill out your copyright notice in the Description page of Project Settings.



#include "Weapon/Rifle.h"
#include "Character/PGBaseCharacter.h"
#include "Animation/AnimMontage.h"
#include "PortGame/PortGame.h"
#include "Engine/DamageEvents.h"
#include "Weapon/BlockAim.h"
#include "Data/WeaponData.h"
#include "Data/GunWeaponData.h"



ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<ABlockAim>
		BlockingAim(TEXT("/Script/Engine.Blueprint'/Game/PortGame/Weapon/BP_BlockAim.BP_BlockAim_C'"));
	if (BlockingAim.Class)
	{
		BlockAimClass = BlockingAim.Class;
	}

}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (OwnerCharacter->GetCurrentIsAiming())
		CheckAimBlock();
	else
		BlockAim->SetActorHiddenInGame(true);*/

}

void ARifle::OnInitializeWeapon(APGBaseCharacter* BaseCharacter, UWeaponData* weaponData)
{
	Super::OnInitializeWeapon(BaseCharacter, weaponData);
	
	if (IsValid(WeaponMesh))
	{
		WeaponStaticComponent->SetStaticMesh(WeaponMesh);

	}
	OwnerCharacter->GetbIshootDelegate().AddUObject(this,
		&ThisClass::ShootCheck);
	OwnerCharacter->GetbIsReloadDelegate().AddUObject(this,
		&ThisClass::Reloading);
	if (weaponData)
	{
		UGunWeaponData* gunWeaponData = Cast<UGunWeaponData>(weaponData);
		WeaponSocket = gunWeaponData->WeaponSocket;
		GunStat = gunWeaponData->GunStat;
		ReloadMontage = gunWeaponData->ReloadMontage;

		SetUpGunStat();
	}
	Currentammo = ammoMaxCount;

	////조준선 블락 생성
	//if (BlockAimClass)
	//{
	//	ABlockAim* spawnBlockAim = GetWorld()->SpawnActor<ABlockAim>(BlockAimClass);
	//	BlockAim = spawnBlockAim;
	//	if (BlockAim)
	//	{
	//		BlockAim->AttachToComponent(WeaponStaticComponent,
	//			FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	//	}
	//}
	
}

void ARifle::Attack()
{
	Super::Attack();
	if (OwnerCharacter->GetCurrentIsShooting())
	{
		if (!FireTimerHandle.IsValid() && !ReloadTimerHandle.IsValid()
			&&CurrentCombo==0)
			FireWithLineTrace();
	}
	else
	{
		if (ReloadTimerHandle.IsValid())
		{
			GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
			UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
			//AnimInstance->Montage_Stop(0.0f, ReloadMontage);
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
		StartFire();
	else
	{
		StopFire();
		
	}
	
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
	UE_LOG(LogTemp, Warning, TEXT("Stop"));
	if (FireTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
	}

}

void ARifle::Reloading()
{
	if (ReloadTimerHandle.IsValid())return;

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ReloadMontage, 1.0f);
	GetWorld()->GetTimerManager().SetTimer(
		ReloadTimerHandle,
		[this]() {
			Currentammo = ammoMaxCount;
			UE_LOG(LogTemp, Warning, TEXT("Reloading"));
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
		UE_LOG(LogTemp, Warning, TEXT("NoArmo"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Time : %lf"), GetWorld()->GetTimeSeconds());
	Currentammo--;
	

	const FVector Camerastart = OwnerCharacter->GetAimLocation();
	//카메라 시작지점
	FVector CameraLoc = OwnerCharacter->GetAimLocation();
	const FVector CameraEnd = CameraLoc + GetWorld()->GetFirstPlayerController()->GetControlRotation().Vector() * (traceDistance);

	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	TArray <AActor*> ignoreActor;
	ignoreActor.Add(this);
	ignoreActor.Add(OwnerCharacter);
	collisionParams.AddIgnoredActors(ignoreActor);

	const UWorld* currentWorld = GetWorld();

	FVector end;
	DrawDebugLine(currentWorld, Camerastart, CameraEnd, FColor::Green, false, 1.0f);
	if (currentWorld)
	{
		bool OutHitResult = currentWorld->LineTraceSingleByChannel(
			hitResult,
			Camerastart,
			CameraEnd,
			ECollisionChannel::ECC_Visibility,
			collisionParams);
		// 명중!
		if (OutHitResult)
		{
		
			FVector ForwardVector = GetWorld()->GetFirstPlayerController()->GetControlRotation().Vector();
			FVector HitLocationWithOffset = hitResult.Location + (ForwardVector * 50.0f);
			end = HitLocationWithOffset;
			//end = hitResult.Location;
			UE_LOG(LogTemp, Warning, TEXT("Hit"));
		}
		else
			end = CameraLoc + GetWorld()->GetFirstPlayerController()->GetControlRotation().Vector() * traceDistance;

	}


	const FVector start = WeaponStaticComponent->GetSocketLocation(WeaponSocket);
	//카메라 시작지점
	//FVector Loc = OwnerCharacter->GetAimLocation();

	//const FVector end = Loc + GetWorld()->GetFirstPlayerController()->GetControlRotation().Vector() *traceDistance;
	
	/*FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	TArray <AActor*> ignoreActor;
	ignoreActor.Add(this);
	ignoreActor.Add(OwnerCharacter);
	collisionParams.AddIgnoredActors(ignoreActor);

	const UWorld* currentWorld = GetWorld();*/
	DrawDebugLine(currentWorld, start, end, FColor::Red, false, 1.0f);
	if (currentWorld)
	{
		bool OutHitResult = currentWorld->LineTraceSingleByChannel(
			hitResult,
			start,
			end,
			ECC_GameTraceChannel1,
			collisionParams);
		// 명중!
		if (OutHitResult)
		{

			FDamageEvent DamageEvent;
			hitResult.GetActor()->TakeDamage(5.0f, DamageEvent, OwnerCharacter->GetController(), this);
		}

	}

	UE_LOG(LogTemp, Warning, TEXT("%d"), Currentammo);


}

void ARifle::CheckAimBlock()
{
	//const FVector start = WeaponStaticComponent->GetSocketLocation(WeaponSocket);
	 const FVector start = OwnerCharacter->GetAimLocation();
	//카메라 시작지점
	FVector Loc = OwnerCharacter->GetAimLocation();
	const FVector end = Loc + GetWorld()->GetFirstPlayerController()->GetControlRotation().Vector() * (traceDistance);

	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	TArray <AActor*> ignoreActor;
	ignoreActor.Add(this);
	ignoreActor.Add(OwnerCharacter);
	collisionParams.AddIgnoredActors(ignoreActor);

	const UWorld* currentWorld = GetWorld();
	
	if (currentWorld)
	{
		bool OutHitResult = currentWorld->LineTraceSingleByChannel(
			hitResult,
			start,
			end,
			ECollisionChannel::ECC_Visibility,
			collisionParams);
		// 명중!
		if (OutHitResult)
		{
			if (BlockAim)
			{
				BlockAim->SetActorHiddenInGame(false);
				BlockAim->SetActorLocation(hitResult.Location);
				TestAim = hitResult.Location;
			}
		}
		else
			BlockAim->SetActorHiddenInGame(true);
	}
}


