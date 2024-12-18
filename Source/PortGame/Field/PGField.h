// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"
#include "PGField.generated.h"

UCLASS()
class PORTGAME_API APGField : public AActor ,public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	
	APGField();

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Field)
	TObjectPtr<class UStaticMeshComponent> FieldMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Field)
	TObjectPtr<class UBoxComponent> AIField;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);


	//Field ÆÀ ¼³Á¤
public:
	FORCEINLINE virtual void SetGenericTeamId(const FGenericTeamId& teamID) override { TeamId = teamID; myteam = teamID; }

	FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const override { return myteam; }

protected:
	void SetTeamColor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 TeamId;

	FGenericTeamId myteam;

	//AI»ý¼º
protected:
	void OnAISpawn();



protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<TObjectPtr<class APGNpcCharacter>> AICharacters;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<TObjectPtr<class UBaseCharacterDataAsset>> AIDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnCount;




};
