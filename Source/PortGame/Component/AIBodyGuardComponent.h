// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIBodyGuardComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnProtectCountChangeDelegate, int32 /*ProtectMePawns.arraynum*/);
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
	virtual bool CanPlayerProtect(APawn* pawn);

	virtual AActor* SetPlayerProtectPawn(APawn* pawn);

	//virtual FVector CalculateOffsetPawn(APawn* pawn);

	UFUNCTION()
	virtual void DeletePlayerProtectPawn(APawn* pawn);


public:
	FORCEINLINE uint8 GetMaxProtectCount() { return MaxProtectcount; }

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<APawn>> ProtectMePawns;

	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<class ADummyPosActor>> PawnsPosActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxProtectcount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffsetX=150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffsetY=150.0f;

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

		
};
