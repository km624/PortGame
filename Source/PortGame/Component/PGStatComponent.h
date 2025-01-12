// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/PGCharacterStat.h"
#include "Portgame/Portgame.h"
#include "PGStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpZeroDelegate, AActor* /*DamageCauser*/);

DECLARE_MULTICAST_DELEGATE(FOnHitGaugeZeroDelegate);
// Hp º¯°æµÊ
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitGaugeChangedDelegate, float /*CurrentHitGauge*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEXPChangedDelegate, float /*EXP*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterLevelChangedDelegate, int32 /*Level*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUltiSkillGaugeChangedDelegate, float /*CurrentUltiSkillGauge*/);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnStatChangedDelegate, const FPGCharacterStat& /*BaseStat*/ , const FPGCharacterStat& /*Modifier*/, const FPGCharacterStat&/*LEvelStat*/);


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
	FOnCharacterLevelChangedDelegate OnLevelChanged;
	FOnEXPChangedDelegate OnEXPChanged;

	

	void SetCurrentRarity(FName rarity);

	FORCEINLINE void SetBaseStat(const FPGCharacterStat& InBaseStat)
	{
		BaseStat = InBaseStat;  OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat(), GetLevelStat());
	}
	FORCEINLINE void SetModifierStat(const FPGCharacterStat& InModifierStat)
	{
		ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat(), GetLevelStat());
	}

	void SetUpPlayerLevel(int32 lelvel);

	FORCEINLINE void SetLevelStat(const FPGCharacterStat& InLevelStat)
	{
		LevelStat = InLevelStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat(), GetLevelStat());
	}

	FORCEINLINE const FPGCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FPGCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE const FPGCharacterStat& GetLevelStat() const { return LevelStat; }

	FORCEINLINE FPGCharacterStat GetTotalStat() const { return BaseStat + ModifierStat + LevelStat; }
	
	
	FORCEINLINE float  GetCurrentHp() { return CurrentHp; }

	
	FORCEINLINE float  GetCurrentHitGauge() { return CurretHitGauge; }
	
	FORCEINLINE float  GetMaxUltiSkillGauge() { return MaxUltiSkillGauge; }
	FORCEINLINE float  GetCurrentUltiSkillGauge() { return CurrentUltiSkillGauge; }

	FORCEINLINE int32 GetCurrentLevel() { return CurrentCharacterLevel; }
	
	
public:

	void Damaged(float Damage, AActor* DamageActor);
	
	void HitGaugeDamaged(float Damage);

	void AddUlitSkillGauge(float AddUltigauge);

	void ResetUlitSkillGauge();

	void AddEXP();
	
protected:
	void SetHp(float NewHp);

	void SetHitGauge(float NewHitGauge);


	void LevelUp();

protected:

	void ResetHitGauge();

	void HitGaugeZeroEffect();
	

	
protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	FName CurrentCharacterRarity;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 CurrentCharacterLevel;

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
	FPGCharacterStat LevelStat;

	/*UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, FPGCharacterStat> AllStat;*/
		
};
