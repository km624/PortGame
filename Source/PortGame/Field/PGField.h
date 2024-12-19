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

	void InitializeField();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Field)
	TObjectPtr<class UStaticMeshComponent> FieldMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Field)
	TObjectPtr<class UBoxComponent> AIField;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);


	//Field 팀 설정
public:
	FORCEINLINE virtual void SetGenericTeamId(const FGenericTeamId& teamID) override { TeamId = teamID; myteam = teamID; }

	FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const override { return myteam; }

protected:
	void SetTeamColor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	uint8 TeamId;

	FGenericTeamId myteam;

	//AI생성
protected:
	void OnAISpawn();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Field")
	TArray<TObjectPtr<class APGNpcCharacter>> AICharacters;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Field")
	TArray<TObjectPtr<class UBaseCharacterDataAsset>> AIDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnCount;


public:
	void DamageFieldGauge(class APawn* deadpawn,int8 attackteamid);

	void ChangedField(int8 teamid);

	//필드 게이지
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	float MaxFieldGague = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Field")
	float currentFieldGauge;

	float FieldDamage= 50.0f;


};
