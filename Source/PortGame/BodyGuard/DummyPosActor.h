// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DummyPosActor.generated.h"

UCLASS()
class PORTGAME_API ADummyPosActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ADummyPosActor();

protected:
	
	virtual void BeginPlay() override;



protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

};
