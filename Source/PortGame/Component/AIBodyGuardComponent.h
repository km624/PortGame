// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/AIAttackEnumData.h"
#include "AIBodyGuardComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnProtectCountChangeDelegate, int32 /*ProtectMePawns.arraynum*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBGGaugeChangeDelegate, float /*CurrentBgGauge*/);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTGAME_API UAIBodyGuardComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIBodyGuardComponent();


protected:
	virtual void InitializeComponent() override;

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void SetUpBodyGuardOptions(TArray<class UBGBaseOptionDataAsset*>& OptionDataAssets);

public:
	virtual bool CanPlayerProtect(APawn* pawn);

	virtual AActor* SetPlayerProtectPawn(APawn* pawn);

	//virtual FVector CalculateOffsetPawn(APawn* pawn);

	UFUNCTION()
	 void DeletePlayerProtectPawn(APawn* pawn);


public:
	FORCEINLINE uint8 GetMaxProtectCount() { return MaxProtectcount; }

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<APawn>> ProtectMePawns;

	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<class ADummyPosActor>> PawnsPosActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxProtectcount = 5;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr <class UBGBaseOptionDataAsset> DefaultOptionDataAsset;


public:
	FOnProtectCountChangeDelegate OnProtectCountChanged;


protected:
	AActor* SpawnPosActor(FVector newlocation);


	void AlignPawnsPosActor();


protected:
	UPROPERTY()
	uint8 currentPosOption = 0;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<TObjectPtr<class UBodyGuardBase>> BodyGuardOptions;

	UPROPERTY()
	TSubclassOf<class ADummyPosActor> PosActorClass;

public:
	void BodyGuardOptionsClick(int32 optionnum);

	void StartBodyGuardLogic(EAIAttackEnumData attackenum, int32 optionnum);

	void ChangeBodyGuardPosition(int optionnum);

protected:
	void SetBGGagueTimer();

	void TimerAddBGGauge();

public:
	bool UseBGOptionGauge(uint8 optiongauge);

	FORCEINLINE int32 GetMaxBGGaugeCount() { return MaxBGGaugeCount; }
	FORCEINLINE float GetCurrentBGGauge() { return CurrentBGGauge; }

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxBGGauge;

	int32 MaxBGGaugeCount = 5;

	float CurrentBGGauge;

	float AddBGGauge = 0.1f;


public:
	FOnBGGaugeChangeDelegate BGGaugeChanaged;
	
	FTimerHandle BGGuageTimer;


public:
	void BoyGuardOptionHover(int32 optionnum);

	void DestroyAllPreviewActors();

protected:
	AActor* SpawnPriviewActors();

	

protected:
	UPROPERTY()
	TSubclassOf<class ADummyPrieviewActor> PreviewActorClass;

	UPROPERTY()
	TArray<TObjectPtr<class ADummyPrieviewActor>> PreviewActors;

	uint8 CurrentHover;


	
	///플레이어가 죽었을때 바디가드들 free  자유를 줘야함
		
};
