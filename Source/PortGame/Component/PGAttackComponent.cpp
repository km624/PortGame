

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
		// SpawnActor를 통해 weapon 데이터를 기반으로 한 액터 생성
		AWeapon* spawnWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);

		Weapon = spawnWeapon;
		SLOG(TEXT("spawnWeapon"));
		APGBaseCharacter* BaseCharacter = Cast<APGBaseCharacter>(GetOwner());


		spawnWeapon->WeaponAttachment(BaseCharacter);
		
	}


}

void UPGAttackComponent::AttackToWeapon()
{
	if (Weapon)
		Weapon->Attack();
}




