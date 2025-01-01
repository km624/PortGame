// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"
#include "PGField.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFieldChanged, int8 /*teamid*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFielclassChanged, class APGField* /*APGField*/);

USTRUCT(BlueprintType)
struct FWeightedAIData
{
	GENERATED_BODY()

	// 데이터 항목
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBaseCharacterDataAsset> AIData;

	// 확률 (0~100)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	FWeightedAIData() : AIData(nullptr), Weight(0.0f) {}
};

UCLASS()
class PORTGAME_API APGField : public AActor ,public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	
	APGField();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float deltatime) override;

public:
	void SetUpField();

protected:
	virtual void InitializeField(uint8 teamid);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AIFieldData)
	TObjectPtr<class UStaticMeshComponent> FieldMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AIFieldData)
	TObjectPtr<class UBoxComponent> AIField;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DrawDistance = 5000.0f;

protected:

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
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

	/*UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AIFieldData")
	TArray<TObjectPtr<class UBaseCharacterDataAsset>> AIDatas;*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIFieldData")
	TArray<FWeightedAIData> AIDatas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIFieldData")
	int32 SpawnCount;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIFieldData")
	TArray<TObjectPtr<class APGPlayerCharacter>> PlayerCharacters;


public:
	void DamageField(class APawn* deadpawn,int8 attackteamid);

	FOnFieldChanged OnFieldChanged;

	FOnFielclassChanged OnFieldClassChanged;

protected:

	virtual void DamageFieldGauge(int8 attackteamid);

	virtual void ChangedField(int8 teamid);

	//필드 게이지
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIFieldData")
	float MaxFieldGague = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIFieldData")
	float currentFieldGauge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIFieldData")
	float FieldDamage= 25.0f;


protected:

	void OnAttackAISpawn();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIFieldData")
	int32 AttackAISpawnCount;

	FTimerHandle AttackAISpawnTimeHandler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIFieldData")
	float AttackAISpawnTime= 60.0f;



};
