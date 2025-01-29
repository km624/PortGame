// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyGuard/DummyPrieviewActor.h"
#include "Components/SkeletalMeshComponent.h"
// Sets default values
ADummyPrieviewActor::ADummyPrieviewActor()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

	SkeletalMeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));

	SkeletalMeshComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ADummyPrieviewActor::BeginPlay()
{
	Super::BeginPlay();
	
}



