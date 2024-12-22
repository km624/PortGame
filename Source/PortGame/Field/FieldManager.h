// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FieldManager.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API UFieldManager : public UObject
{
	GENERATED_BODY()


protected:
	UFieldManager();

public:
	void InitializeFieldManager();

public:

	FVector FoundEnemyField(class AActor* sourceActor);

protected:


	void FindAllFields();


	void StartAllFields();

	void CheckLastField(int8 teamid);

	void SetLastFieldLock(int8 teamid ,bool lock);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Field")
	TArray<TObjectPtr<class APGField>> Fields;
	
};
