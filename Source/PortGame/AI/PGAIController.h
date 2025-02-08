// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Data/AIAttackEnumData.h"
#include "Interface/AIControllerInterface.h"
#include "PGAIController.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API APGAIController : public AAIController, public IAIControllerInterface
{
	GENERATED_BODY()

public:
	APGAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:

	void SetMyFieldData(class APGField* field);

	void SetAttackAIData();

	virtual void SetForceMoveVector(FVector targetVector) override;

	virtual void SetBodyGuardAttack(EAIAttackEnumData attackenum,FRotator attackrotator) override;

	void SetVisible(bool visible);

	void RunAI();

	void BindGameStart();

	void CheckGameStart();

	void SetGameStart(bool bgameStart);


	void TOMyFieldDead(int8 teamid);

	void AIIsDead();

public:
	
	void StopAI();

	void BlackBoardReset();

protected:
	virtual void BeginPlay()override;

	virtual void OnPossess(APawn* pawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
	
protected:
	virtual void StopTree() override;

	virtual void StartTree() override;

protected:
	/*UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<class UCrowdFollowingComponent> CrowdFollingComponent;*/
};
