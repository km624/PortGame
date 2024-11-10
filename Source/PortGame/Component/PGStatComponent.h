// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/PGCharacterStat.h"
#include "PGStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

DECLARE_MULTICAST_DELEGATE(FOnHitGaugeZeroDelegate);
// Hp º¯°æµÊ
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitGaugeChangedDelegate, float /*CurrentHitGauge*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterLevelChangedDelegate, float /*Level*/);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnStatChangedDelegate, const FPGCharacterStat& /*BaseStat*/ , const FPGCharacterStat& /*Modifier*/,const float/*maxHutGauge*/);





UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTGAME_API UPGStatComponent : public UActorComponent
{
	GENERATED_BODY()

private:

public:	
	
	UPGStatComponent();



protected:
	virtual void InitializeComponent() override;

	

public:
	FOnHpChangedDelegate OnHpChanged;
	FOnHpZeroDelegate OnHpZero;
	FOnStatChangedDelegate OnStatChanged;
	FOnHitGaugeZeroDelegate OnHitGaugeZero;
	FOnHitGaugeChangedDelegate OnHitGaugeChanged;

	void SetCurrentRarity(FName rarity);

	FORCEINLINE void SetBaseStat(FName rarity) 
	{
		BaseStat = AllStat[rarity]; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat(), GetMaxHitGauge());
	}
	FORCEINLINE void SetModifierStat(const FPGCharacterStat& InModifierStat)
		{ ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat(), GetMaxHitGauge()); }

	FORCEINLINE const FPGCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FPGCharacterStat& GetModifierStat() const { return ModifierStat; }

	FORCEINLINE FPGCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	
	
	FORCEINLINE float  GetCurrentHp() { return CurrentHp; }

	FORCEINLINE void SetMaxHitGauge(float NewMaxHitGuage) { MaxHitGauge = NewMaxHitGuage; }
	FORCEINLINE float  GetMaxHitGauge() { return MaxHitGauge; }
	FORCEINLINE float  GetCurrentHitGauge() { return CurretHitGauge; }
	

	void ResetHitGauge();

	void Damaged(float Damage);
	
protected:
	void SetHp(float NewHp);

	void SetHitGauge(float NewHitGauge);

	void SetLevelCharacter(int32 level);
	
protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	FName CurrentCharacterRarity;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentCharacterLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float EXP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float MaxHitGauge;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurretHitGauge;

	

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta= (AllowPrivateAccess="true"))
	FPGCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPGCharacterStat ModifierStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, FPGCharacterStat> AllStat;
		
};
