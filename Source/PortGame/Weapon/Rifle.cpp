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

	

	if (weaponData)
	{
		UGunWeaponData* gunWeaponData = Cast<UGunWeaponData>(weaponData);
		WeaponSocket = gunWeaponData->WeaponSocket;
		GunStat = gunWeaponData->GunStat;
		ReloadMontage = gunWeaponData->ReloadMontage;
		CameraShakeClass = gunWeaponData->CameraShakeClass;
		bIsRifle = gunWeaponData->bIsRifle;
		OwnerCharacter->SetbIsRilfe(bIsRifle);
		NAGunEffect = gunWeaponData->NAGunEffect;

		GunNiagaraComponent->SetAsset(NAGunEffect);
		FVector SocketLocation = WeaponStaticComponent->GetSocketLocation(FireLocation);
		GunNiagaraComponent->SetRelativeLocation(SocketLocation);

		SetUpGunStat();

		ReloadMontageTime = ReloadMontage->GetPlayLength() / reloadingTime - 0.3f;
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
			OnbIsGunReload.Broadcast(false);
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

	bIsGunReloaded = true;
	OwnerCharacter->SetbIsReload(bIsGunReloaded);
	OnbIsGunReload.Broadcast(bIsGunReloaded);

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ReloadMontage, reloadingTime);


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
	OnbIsGunReload.Broadcast(bIsGunReloaded);
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
	//카메라 시작지점
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
				end = CameraEnd;


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


	//근접의 5분의1 데미지
	if (OwnerCharacter->GetPlayerCharacterType() == EPlayerCharacterType::Nikke)
		GunDamage = OwnerCharacter->GetTotalStat().Attack * 0.5f;
	else
		GunDamage = OwnerCharacter->GetTotalStat().Attack * 0.2f;

	if (!bIsRifle)
		GunDamage *= 2.5f;

	if (OwnerCharacter->GetbIsNikkeSkill())
		GunDamage *= 3.0f;

	//AI는 거기서 더 4분의 데미지
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
			OwnerCharacter->AddUltiSkillGaugeToComp(GunDamage);
		}

	}
	StartGunEffect();


}

void ARifle::StartGunEffect()
{
	if (NAGunEffect)
	{
		SLOG(TEXT("GunEffect"));
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




