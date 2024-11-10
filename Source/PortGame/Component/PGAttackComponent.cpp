

#include "Component/PGAttackComponent.h"
#include "Weapon/Weapon.h"
#include "Character/PGBaseCharacter.h"
#include "PortGame/PortGame.h"
#include "Data/WeaponData.h"

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






