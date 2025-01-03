// Fill out your copyright notice in the Description page of Project Settings.


#include "Field/PGLastField.h"
#include "Character/PGPlayerCharacter.h"
#include "UI/PGHudWidget.h"

APGLastField::APGLastField() : bIsLocked(true)
{

}

void APGLastField::SetbIsLocked(bool lock)
{
	
	bIsLocked = lock;

	PlayerLockHUDUpdate(bIsLocked);
}

void APGLastField::PlayerLockHUDUpdate(bool lock)
{
	if (PlayerCharacters.Num() > 0)
	{
		for (TObjectPtr<APGPlayerCharacter>& palyerCharacter : PlayerCharacters)
		{
			if (palyerCharacter)
			{
				palyerCharacter->GetPlayerHudWidget()->SetUpLockImage(lock);
			}
		}
	}
}

void APGLastField::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepHitResult);
	
	PlayerLockHUDUpdate(bIsLocked);

}

void APGLastField::DamageFieldGauge(int8 attackteamid)
{
	if (bIsLocked) return;

	Super::DamageFieldGauge(attackteamid);
}

void APGLastField::ChangedField(int8 teamid)
{
	Super::ChangedField(teamid);
}



