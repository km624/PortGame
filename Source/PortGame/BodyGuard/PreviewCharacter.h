// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PreviewCharacter.generated.h"

UCLASS()
class PORTGAME_API APreviewCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	APreviewCharacter();

protected:
	
	virtual void BeginPlay() override;

	


	
public:

	void SetAnimation(class UAnimationAsset* animasset);

};
