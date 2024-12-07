// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Loop.h"
#include "BTDecorator_RandomLoop.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTDecorator_RandomLoop : public UBTDecorator_Loop
{
	GENERATED_BODY()

public:
	UBTDecorator_RandomLoop();
protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;

	UPROPERTY(EditAnywhere, Category = "RandomLoop")
	int32 MinLoops;

	UPROPERTY(EditAnywhere, Category = "RandomLoop")
	int32 MaxLoops;
};
