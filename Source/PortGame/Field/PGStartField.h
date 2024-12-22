// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Field/PGLastField.h"
#include "PGStartField.generated.h"

/**
 * 
 */
UCLASS()
class PORTGAME_API APGStartField : public APGLastField
{
	GENERATED_BODY()

public:
	APGStartField();

protected:
	virtual void InitializeField(uint8 teamid) override;

	void SetupPlayerbleCharacter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
	TArray<TObjectPtr<class UPlayerCharacterDataAsset>> PlayerCharacterData;




	
};
