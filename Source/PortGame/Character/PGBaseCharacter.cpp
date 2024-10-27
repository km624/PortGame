// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PGBaseCharacter.h"

// Sets default values
APGBaseCharacter::APGBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APGBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APGBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APGBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

