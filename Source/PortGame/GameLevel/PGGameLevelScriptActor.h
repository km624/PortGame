// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "PGGameLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API APGGameLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	APGGameLevelScriptActor();
	

protected:
	virtual void BeginPlay() override;


public:

	class APGField* FoundEnemyField(class APGField* field);

protected:


	void FindAllFields();


	void StartAllFields();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Field")
	TArray<TObjectPtr<class APGField>> Fields;
	
};
