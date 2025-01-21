// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyGuard/DummyPosActor.h"
#include "Components/StaticMeshComponent.h"

ADummyPosActor::ADummyPosActor()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

void ADummyPosActor::BeginPlay()
{
	Super::BeginPlay();
	
}




