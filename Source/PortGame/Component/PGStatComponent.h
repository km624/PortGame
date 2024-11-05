// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/PGCharacterStat.h"
#include "PGStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
// Hp ∫Ø∞Êµ 
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatChangedDelegate, FPGCharacterStat /*Stat*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterLevelChangedDelegate, float /*Level*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTGAME_API UPGStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPGStatComponent();

protected:
	virtual void InitializeComponent() override;

	
protected:
	void SetHp(float NewHp);

	void SetStat(FName rarity);

	void SetLevelCharacter(int32 level);
	


public:
	FOnHpChangedDelegate OnHpChanged;
	FOnHpZeroDelegate OnHpZero;

	void Damaged(float Damage);

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;
	
	////¿”Ω√ maxp
	//UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	//float MaxHp=100.0f;S

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentCharacterLevel;


	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta= (AllowPrivateAccess="true"))
	FPGCharacterStat BaseStat;
		
};
