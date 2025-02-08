// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Interface/FieldManagerInterface.h"
#include "Interface/ObjectPoolingInterface.h"
#include "Interface/LevelGameStartInterface.h"
#include "PGGameLevelScriptActor.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStarted, bool /*bGameStart*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCountdownUpdated, int32 /*CountdownValue*/);

UCLASS()
class PORTGAME_API APGGameLevelScriptActor : public ALevelScriptActor, public IFieldManagerInterface,public IObjectPoolingInterface , public ILevelGameStartInterface
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


protected:
	virtual void SetGameStartPlayer(class APGPlayerController* playercontroller) override;

	virtual void SetGameStartAI(class APGAIController* aicontroller) override;

	virtual void SetGameStartTimer() override;
	
	void CheckCount();

	virtual int32 GetStartCount() override;
	
	virtual bool GetbGameStart() override;



protected:
	UPROPERTY()
	uint8 bGameStart : 1;

	FOnGameStarted OnGameStarted;

	FOnCountdownUpdated OnCountValueChanged;

	FTimerHandle GameStartTimer;

	UPROPERTY()
	int32 GameStartCount = 3;

	


};
