// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindProtectPos.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UBTService_FindProtectPos : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_FindProtectPos();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	/*UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float OffsetX;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float OffsetY;*/

	
};
