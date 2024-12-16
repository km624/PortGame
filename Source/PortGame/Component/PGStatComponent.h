// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/PGCharacterStat.h"
#include "Portgame/Portgame.h"
#include "PGStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

DECLARE_MULTICAST_DELEGATE(FOnHitGaugeZeroDelegate);
// Hp ∫Ø∞Êµ 
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitGaugeChangedDelegate, float /*CurrentHitGauge*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterLevelChangedDelegate, float /*Level*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUltiSkillGaugeChangedDelegate, float /*CurrentUltiSkillGauge*/);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FPGCharacterStat& /*BaseStat*/ , const FPGCharacterStat& /*Modifier*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTGAME_API UPGStatComponent : public UActorComponent
{
	GENERATED_BODY()

private:

public:	
	
	UPGStatComponent();


	//DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnStatChangedDelegate, const FPGCharacterStat& /*BaseStat*/ , const FPGCharacterStat& /*Modifier*/,const float/*maxHutGauge*/);
protected:
	virtual void InitializeComponent() override;

	

public:
	FOnHpChangedDelegate OnHpChanged;
	FOnHpZeroDelegate OnHpZero;
	FOnStatChangedDelegate OnStatChanged;
	FOnHitGaugeZeroDelegate OnHitGaugeZero;
	FOnHitGaugeChangedDelegate OnHitGaugeChanged;
	FOnUltiSkillGaugeChangedDelegate OnUltiSkillGaugechanged;

	

	void SetCurrentRarity(FName rarity);

	FORCEINLINE void SetBaseStat(FName rarity) 
	{
		BaseStat = AllStat[rarity];  OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}
	FORCEINLINE void SetModifierStat(const FPGCharacterStat& InModifierStat)
	{
		ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}

	FORCEINLINE const FPGCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FPGCharacterStat& GetModifierStat() const { return ModifierStat; }

	FORCEINLINE FPGCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	
	
	FORCEINLINE float  GetCurrentHp() { return CurrentHp; }

	
	FORCEINLINE float  GetCurrentHitGauge() { return CurretHitGauge; }
	
	FORCEINLINE float  GetMaxUltiSkillGauge() { return MaxUltiSkillGauge; }
	FORCEINLINE float  GetCurrentUltiSkillGauge() { return CurrentUltiSkillGauge; }
	
	
public:

	void Damaged(float Damage);
	
	void HitGaugeDamaged(float Damage);

	void AddUlitSkillGauge(float AddUltigauge);

	void ResetUlitSkillGauge();


	//¿”Ω√
//protected:
	void SetHp(float NewHp);

	void SetHitGauge(float NewHitGauge);

	void SetLevelCharacter(int32 level);

protected:

	void ResetHitGauge();

	void HitGaugeZeroEffect();
	

	
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
	float CurretHitGauge;

	FTimerHandle ResetHitGaugeTimer;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentUltiSkillGauge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float addPrecentUlitSkillGuage = 10.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Stat)
	float MaxUltiSkillGauge = 100.0f;
	

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta= (AllowPrivateAccess="true"))
	FPGCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPGCharacterStat ModifierStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, FPGCharacterStat> AllStat;
		
};
