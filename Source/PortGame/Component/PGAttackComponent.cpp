

#include "Component/PGAttackComponent.h"
#include "Weapon/Weapon.h"
#include "Character/PGBaseCharacter.h"
#include "PortGame/PortGame.h"
#include "Components/CapsuleComponent.h"
#include "Data/WeaponData.h"
#include "Engine/DamageEvents.h"
#include "Interface/AttackHitStopInterface.h"

UPGAttackComponent::UPGAttackComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

}


void UPGAttackComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	SetWeaponClass();

	SetUpWeapon();
}

void UPGAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	//SetUpWeapon();
}

void UPGAttackComponent::SetWeaponClass()
{
	if(WeaponData)
		WeaponClass = WeaponData->WeaponClass;
}

void UPGAttackComponent::SetUpWeapon()
{
	if (WeaponClass)
	{
		
		AWeapon* spawnWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);

		Weapon = spawnWeapon;
		SLOG(TEXT("spawnWeapon"));
		APGBaseCharacter* BaseCharacter = Cast<APGBaseCharacter>(GetOwner());


		spawnWeapon->OnInitializeWeapon(BaseCharacter, WeaponData);

		spawnWeapon->AttachToComponent(BaseCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, spawnWeapon->GetWeaponFname());
		
		BaseCharacter->SetUpModifierStat(spawnWeapon->ModifierStat);
	}


}

void UPGAttackComponent::AttackToWeapon()
{
	if (Weapon)
		Weapon->Attack();
}

void UPGAttackComponent::AttackHitCheck()
{
	TArray<FHitResult> OutHitResults;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, GetOwner());

	APGBaseCharacter* BaseCharacter = Cast<APGBaseCharacter>(GetOwner());
	const float AttackRange = BaseCharacter->GetTotalStat().AttackRange;
	const float AttackRadius = BaseCharacter->GetTotalStat().AttackRange;
	const float AttackDamage = BaseCharacter->GetTotalStat().Attack;

	const FVector Start = BaseCharacter->GetActorLocation() + BaseCharacter->GetActorForwardVector() * BaseCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

	const FVector End = Start + BaseCharacter->GetActorForwardVector() * AttackRange;


	bool HitDetected = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	if (HitDetected)
	{

		for (const FHitResult& Hit : OutHitResults)
		{
			if (Hit.GetActor())
			{
				IAttackHitStopInterface* playerCharacter = Cast<IAttackHitStopInterface>(GetOwner());
				if (playerCharacter)
				{
					playerCharacter->AttackHitStop();
				}
				FDamageEvent DamageEvent;
				Hit.GetActor()->TakeDamage(AttackDamage, DamageEvent, BaseCharacter->GetController(), BaseCharacter);

			}
		}
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	FVector TraceVec = BaseCharacter->GetActorForwardVector() * AttackRange;
	FVector Center = BaseCharacter->GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	float DebugLifeTime = 5.0f;

	// SweepMulti�� ������ ������ �׷���
	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif
}






