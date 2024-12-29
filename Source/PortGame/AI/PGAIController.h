// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/AIControllerInterface.h"
#include "PGAIController.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API APGAIController : public AAIController,public IAIControllerInterface
{
	GENERATED_BODY()

public:
	APGAIController();

public:

	void SetMyFieldData(class APGField* field);

	void SetAttackAIData();

	void SetForceMoveVector(FVector targetVector);

	void RunAI();

	void TOMyFieldDead(int8 teamid);

	void AIIsDead();

public:
	
	void StopAI();

	void BlackBoardReset();

protected:
	virtual void OnPossess(APawn* pawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
	
protected:
	virtual void StopTree() override;

	virtual void StartTree() override;
};
