// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/PGCharacterStat.h"
#include "PGStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
// Hp º¯°æµÊ
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatChangedDelegate, FPGCharacterStat /*Stat*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterLevelChangedDelegate, float /*Level*/);

UENUM(BlueprintType)
enum class EPlayerRarity :uint8
{
	Normal = 0,
	Rare
};

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
	FORCEINLINE void SetBaseStat(FName rarity) { BaseStat = AllStat[rarity]; }
	FORCEINLINE const FPGCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE float  GetCurrentHp() { return CurrentHp; }

	void Damaged(float Damage);
	
protected:
	void SetHp(float NewHp);

	//void SetStat(FName rarity);

	void SetLevelCharacter(int32 level);
	
protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;
	

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentCharacterLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float EXP;


	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta= (AllowPrivateAccess="true"))
	FPGCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, FPGCharacterStat> AllStat;
		
};
