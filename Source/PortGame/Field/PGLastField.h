// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Field/PGField.h"
#include "PGLastField.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API APGLastField : public APGField
{
	GENERATED_BODY()

public:
	
	APGLastField();

	FORCEINLINE bool GetbIsLock() { return bIsLocked; }

	void SetbIsLocked(bool lock);

protected:
	void PlayerLockHUDUpdate(bool lock);

	
protected:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult) override;

	virtual void DamageFieldGauge(int8 attackteamid) override;

	virtual void ChangedField(int8 teamid) override;


protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	uint8 bIsLocked : 1;
};
