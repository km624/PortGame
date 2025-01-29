// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DummyPrieviewActor.generated.h"

UCLASS()
class PORTGAME_API ADummyPrieviewActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ADummyPrieviewActor();

protected:
	
	virtual void BeginPlay() override;

	virtual void Tick(float dt) override;

public:

	void SetAnimation(class UAnimationAsset* animasset);
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <class USceneComponent> RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCapsuleComponent> Capsule;

};
