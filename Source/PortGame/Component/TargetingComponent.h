// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"

USTRUCT(BlueprintType)
struct FTargetDistance
{
	GENERATED_BODY()

	AActor* Target; 
	float Distance;      

	
	bool operator<(const FTargetDistance& Other) const
	{
		return Distance < Other.Distance;
	}
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTGAME_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UTargetingComponent();

protected:

	virtual void InitializeComponent() override;

	virtual void BeginPlay() override;


public:

	FORCEINLINE class AActor* GetTargetActor() { return TargetActor; }
	FORCEINLINE bool GetbIsTargetLock() { return bIsTargetLock; }

	void SetTargetLock();

	void FindClosestEnemy();

	void TargetLockOn( float dt);

	float CharcterTargetDistance();

protected:

	void FindClosestEnemy();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta = (AllowPrivateAccess = "true"))
	uint8 bIsTargetLock : 1;

	// 검색 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta = (AllowPrivateAccess = "true"))
	float SearchDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> TargetActor;

	TArray<AActor*> AllTargetActor;
		
};
