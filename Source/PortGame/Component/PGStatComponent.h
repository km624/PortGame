// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PGStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
// Hp º¯°æµÊ
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);


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


protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;
		
};
