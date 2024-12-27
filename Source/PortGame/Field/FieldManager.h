// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FieldManager.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFieldIndexChanged, int8 /*index*/,bool /*bislock*/);
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

	void CheckWhoIsChanged(class APGField* changefield);

	void CheckLastField(int8 teamid);

	void SetLastFieldLock(int8 teamid ,bool lock);

public:
	FOnFieldIndexChanged OnfieldIndexchanged;

	const TArray<class APGField*>& GetFields() { return Fields; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Field")
	TArray<TObjectPtr<class APGField>> Fields;
	
};
