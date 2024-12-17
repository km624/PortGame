// Fill out your copyright notice in the Description page of Project Settings.



#include "Weapon/Rifle.h"
#include "Character/PGBaseCharacter.h"
#include "Animation/AnimMontage.h"
#include "PortGame/PortGame.h"
#include "Engine/DamageEvents.h"
#include "Data/WeaponData.h"
#include "Data/GunWeaponData.h"
#include "Interface/PlayerCameraShakeInterface.h"
#include "Physics/PGCollision.h"
#include "Interface/AttackHitStopInterface.h"
#include "Character/PGNpcCharacter.h"
#include "Data/CharacterEnumData.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

const FString ARifle::ReloadMontage = TEXT("ReloadingMontage");

ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;

	GunNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GunNiagara"));
	GunNiagaraComponent->SetupAttachment(RootComponent);
	GunNiagaraComponent->bAutoActivate = false;


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
		&ThisClass::StartReloading);
	OwnerCharacter->OnbIsAim.AddUObject(this,
		&ThisClass::InGunRange);
	

	if (weaponData)
	{
		UGunWeaponData* gunWeaponData = Cast<UGunWeaponData>(weaponData);

		WeaponSocket = gunWeaponData->WeaponSocket;
		GunStat = gunWeaponData->GunStat;
		//ReloadMontage = gunWeaponData->ReloadMontage;
		CameraShakeClass = gunWeaponData->CameraShakeClass;
		bIsRifle = gunWeaponData->bIsRifle;
		OwnerCharacter->SetbIsRilfe(bIsRifle);

		if (bIsRifle)
			WeaponSocket = TEXT("weaponRifleSocket");
		else
			WeaponSocket = TEXT("weaponPistolSocket");

		NAGunEffect = gunWeaponData->NAGunEffect;

		OwnerCharacter->LoadAndPlayMontageByPath(OwnerCharacter->CharacterName, ReloadMontage);

		GunNiagaraComponent->SetAsset(NAGunEffect);
		FVector SocketLocation = WeaponStaticComponent->GetSocketLocation(FireLocation);
		GunNiagaraComponent->SetRelativeLocation(SocketLocation);

		SetUpGunStat();

		//ReloadMontageTime = FirstReloadMontage->GetPlayLength() / reloadingTime - 0.3f;
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
			bIsGunReloaded = false;
			OwnerCharacter->SetbIsReload(bIsGunReloaded);
			OnbIsGunReload.Broadcast(false, ReloadMontageTime);
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

void ARifle::InGunRange(bool bisaim)
{
	if (bisaim == false) return;
	const FVector Camerastart = OwnerCharacter->GetAimLocation();
	
	FVector CameraLoc = OwnerCharacter->GetAimLocation();

	const FVector CameraEnd = CameraLoc + OwnerCharacter->GetController()->GetControlRotation().Vector() * (traceDistance);

	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	TArray <AActor*> ignoreActor;
	ignoreActor.Add(this);
	ignoreActor.Add(OwnerCharacter);
	collisionParams.AddIgnoredActors(ignoreActor);

	const UWorld* currentWorld = GetWorld();

	//DrawDebugLine(currentWorld, Camerastart, CameraEnd, FColor::Yellow, false, 1.0f);

	//�÷��̾����� Ȯ��
	IAttackHitStopInterface* Player = Cast<IAttackHitStopInterface>(OwnerCharacter);
	if (Player)
	{
		if (currentWorld)
		{
			bool OutHitResult = currentWorld->LineTraceSingleByChannel(
				hitResult,
				Camerastart,
				CameraEnd,
				CCHANNEL_PGACTION,
				collisionParams);

			if (OutHitResult)
				bInGunRange = true;
			else
				bInGunRange = false;

			OnbInGunRanged.Broadcast(bInGunRange);

		}
	}
}

void ARifle::ShootCheck(bool bIsShoot)
{
	if (bIsShoot && !bIsGunReloaded)
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

void ARifle::StartReloading()
{
	if (ReloadTimerHandle.IsValid())return;

	ReloadMontageTime = OwnerCharacter->AllMontage[ReloadMontage]->GetPlayLength() / reloadingTime - 0.3f;

	bIsGunReloaded = true;
	OwnerCharacter->SetbIsReload(bIsGunReloaded);
	OnbIsGunReload.Broadcast(bIsGunReloaded, ReloadMontageTime);

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(OwnerCharacter->AllMontage[ReloadMontage], reloadingTime);


	GetWorld()->GetTimerManager().SetTimer(
		ReloadTimerHandle,
		this, &ThisClass::EndReloading
		, ReloadMontageTime, false
	);
}

void ARifle::EndReloading()
{

	Currentammo = ammoMaxCount;
	OnAmmoChanged.Broadcast(Currentammo);
	bIsGunReloaded = false;
	OwnerCharacter->SetbIsReload(bIsGunReloaded);
	OnbIsGunReload.Broadcast(bIsGunReloaded, ReloadMontageTime);
	ReloadTimerHandle.Invalidate();
}


void ARifle::FireWithLineTrace()
{

	if (Currentammo <= 0)
	{
		StopFire();
		StartReloading();

		return;
	}

	Currentammo--;
	OnAmmoChanged.Broadcast(Currentammo);

	const FVector Camerastart = OwnerCharacter->GetAimLocation();
	//ī�޶� ��������
	FVector CameraLoc = OwnerCharacter->GetAimLocation();

	const FVector CameraEnd = CameraLoc + OwnerCharacter->GetController()->GetControlRotation().Vector() * (traceDistance);

	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	TArray <AActor*> ignoreActor;
	ignoreActor.Add(this);
	ignoreActor.Add(OwnerCharacter);
	collisionParams.AddIgnoredActors(ignoreActor);

	const UWorld* currentWorld = GetWorld();

	FVector end;
	DrawDebugLine(currentWorld, Camerastart, CameraEnd, FColor::Green, false, 1.0f);

	//�÷��̾����� Ȯ��
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
				end = CameraEnd;


		}
	}
	else
	{
		//�̶��� NPC
		APGNpcCharacter* NPCPlayer = Cast<APGNpcCharacter>(OwnerCharacter);
		if (NPCPlayer)
		{
			end = NPCPlayer->GetTargetPawn()->GetActorLocation();
		}
	}

	if (!bIsRifle)
		GunDamage = OwnerCharacter->GetTotalStat().Attack* 1.5f;
	else
		GunDamage = OwnerCharacter->GetTotalStat().Attack * 0.5f;

	//������ 5����1 ������
	if (OwnerCharacter->GetPlayerCharacterType() == EPlayerCharacterType::BlueArchive)
		GunDamage *= 0.75f;


	if (OwnerCharacter->GetbIsNikkeSkill())
		GunDamage *= 3.0f;

	//AI�� �ű⼭ �� 4���� ������
	if (OwnerCharacter->ActorHasTag(TAG_AI))
		GunDamage *= 0.25f;

	const FVector start = WeaponStaticComponent->GetSocketLocation(FireLocation);
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
			hitResult.GetActor()->TakeDamage(GunDamage, DamageEvent, OwnerCharacter->GetController(), OwnerCharacter);

			APGBaseCharacter* hitTarget = Cast<APGBaseCharacter>(hitResult.GetActor());
			if(hitTarget)
				hitTarget->StartNiagaraEffect(NAGunEffect, hitTarget->GetActorLocation());

			OwnerCharacter->AddUltiSkillGaugeToComp(GunDamage);
		}

	}
	StartGunEffect();


}

void ARifle::StartGunEffect()
{
	if (NAGunEffect)
	{
		//SLOG(TEXT("GunEffect"));
		if (GunNiagaraComponent->IsActive())
			GunNiagaraComponent->Deactivate();
		GunNiagaraComponent->Activate();
	}

	IPlayerCameraShakeInterface* gunrecoil = Cast<IPlayerCameraShakeInterface>(OwnerCharacter->GetController());
	if (gunrecoil)
	{
		gunrecoil->PlayCameraShake(CameraShakeClass);
	}
}




