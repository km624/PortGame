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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AIFieldData)
	TObjectPtr<class UStaticMeshComponent> FieldMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AIFieldData)
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
	//팀 색상을 동적 변경 위함
	UMaterialInstanceDynamic* DynamicMaterial;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIFieldData")
	uint8 TeamId;

	FGenericTeamId myteam;

	//AI생성
protected:
	void OnAISpawn();

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "AIFieldData")
	TArray<TObjectPtr<class APGNpcCharacter>> AICharacters;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AIFieldData")
	TArray<TObjectPtr<class UBaseCharacterDataAsset>> AIDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIFieldData")
	int32 SpawnCount;


public:
	void DamageFieldGauge(class APawn* deadpawn,int8 attackteamid);

	void ChangedField(int8 teamid);

	//필드 게이지
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIFieldData")
	float MaxFieldGague = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIFieldData")
	float currentFieldGauge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIFieldData")
	float FieldDamage= 50.0f;


};
