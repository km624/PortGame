// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MyBTService.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UMyBTService : public UBTService
{
	GENERATED_BODY()
public:
	UMyBTService();
protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DetectRange;
	
};
