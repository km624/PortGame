// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PGAIController.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API APGAIController : public AAIController
{
	GENERATED_BODY()

public:
	APGAIController();

public:
	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* pawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
	
};
