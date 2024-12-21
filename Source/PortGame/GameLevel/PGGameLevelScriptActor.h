// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Interface/FieldManagerInterface.h"
#include "Interface/ObjectPoolingInterface.h"
#include "PGGameLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API APGGameLevelScriptActor : public ALevelScriptActor, public IFieldManagerInterface,public IObjectPoolingInterface
{
	GENERATED_BODY()

public:
	APGGameLevelScriptActor();
	
protected:
	virtual void BeginPlay() override;

protected:
	virtual UFieldManager* GetFieldManager() const override;
	
	virtual UObjectPoolManager* GetObjectPoolManager() const override;



protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<class UFieldManager> FieldManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr <class UObjectPoolManager> ObjectPoolManager;

};
