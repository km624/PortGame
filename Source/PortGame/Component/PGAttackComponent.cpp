

#include "Component/PGAttackComponent.h"
#include "Weapon/Weapon.h"
#include "Character/PGBaseCharacter.h"
#include "PortGame/PortGame.h"

UPGAttackComponent::UPGAttackComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;
	//bWantsInitializeComponent = true;

}


void UPGAttackComponent::InitializeComponent()
{

}

void UPGAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	SetUpWeapon();
}

void UPGAttackComponent::SetUpWeapon()
{
	if (WeaponClass)
	{
		
		AWeapon* spawnWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);

		Weapon = spawnWeapon;
		SLOG(TEXT("spawnWeapon"));
		APGBaseCharacter* BaseCharacter = Cast<APGBaseCharacter>(GetOwner());


		spawnWeapon->OnInitializeWeapon(BaseCharacter);

		spawnWeapon->AttachToComponent(BaseCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, spawnWeapon->GetWeaponFname());
		
	}


}

void UPGAttackComponent::AttackToWeapon()
{
	if (Weapon)
		Weapon->Attack();
}




