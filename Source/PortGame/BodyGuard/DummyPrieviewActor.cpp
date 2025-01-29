// Fill out your copyright notice in the Description page of Project Settings.


#include "BodyGuard/DummyPrieviewActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimationAsset.h"
#include "Components/CapsuleComponent.h"
// Sets default values
ADummyPrieviewActor::ADummyPrieviewActor()
{

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	Capsule->SetCollisionProfileName(TEXT("OverlapAll"));
	Capsule->InitCapsuleSize(42.f, 96.0f);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

	SkeletalMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	SkeletalMeshComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ADummyPrieviewActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADummyPrieviewActor::Tick(float dt)
{
	Super::Tick(dt);
	
}

void ADummyPrieviewActor::SetAnimation(UAnimationAsset* animasset)
{
	SkeletalMeshComponent->PlayAnimation(animasset, true);
	
	SkeletalMeshComponent->SetPlayRate(4.0f);
}



