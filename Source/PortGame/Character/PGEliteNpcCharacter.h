// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PGNpcCharacter.h"
#include "Interface/SetArmorWidgetInterface.h"
#include "Interface/ExecutionEliteNPCInterface.h"
#include "PGEliteNpcCharacter.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnArmorChangedDelegate,uint8/*currentArmor*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerDistanceDelegate, bool/*bInPlayerDistance*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInExecutionRangeDelegate, bool/*bInExecutionRange*/)
UCLASS()
class PORTGAME_API APGEliteNpcCharacter : public APGNpcCharacter,public ISetArmorWidgetInterface,public IExecutionEliteNPCInterface
{
	GENERATED_BODY()


public:
	
	APGEliteNpcCharacter();
	
	//기존 
protected:
	virtual void SetupCharacterData(class UBaseCharacterDataAsset* characterdata) override;

	virtual void SetDead(AActor* DamageCauser) override;

	virtual void WidgetHide();

	virtual void BeginPlay() override;

	virtual void Tick(float deltatime) override;

protected:
	UPROPERTY()
	uint8 bInPlayerDistance : 1;

	UPROPERTY()
	float CurrentPlayerDistance;

	UPROPERTY()
	float MaxPlayerDistance = 3000.0f;

	FOnPlayerDistanceDelegate OnPlayerDistanced;

	// 방어력
protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void HitMontageEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)override;

	void SuperGroggySetTime();
	
	void SuperGroggyEndPlay();

	void SuperGroggyEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxArmor = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 CurrentArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxArmorPercent = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentArmorPercent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bIsSuperGroggy : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoveryArmorTime = 5.0f;

	FOnMontageEnded GroggyEndDelegate;

	UPROPERTY()
	FTimerHandle RecoveryArmorTimer;

	FOnArmorChangedDelegate ArmorChanged;

	

protected:
	virtual void SetArmorWidget(class UUserWidget* userwidget) override;

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<class UArmorWidgetComponent> ArmorWidgetComponent;

protected:
	const static FString SuperGroggyMontage;


protected:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual AActor* OnExecutionStart(AActor* player) override;

	void SetExcutionLocation(AActor* player);

	void PlayExcutionMontage();

	void ForceExecutionHitStop(float time);
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr <class USphereComponent> ExecutionTrigger;

	const static FString ExcuisonHitMontage;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	uint8 InExecutionRange:1;

	FOnInExecutionRangeDelegate OnExecutionRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bIsExecution : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> ExecutionPlayer;
};
