// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLevel/PGGameLevelScriptActor.h"
#include "Field/PGField.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "PortGame/PortGame.h"
#include "Field/FieldManager.h"

APGGameLevelScriptActor::APGGameLevelScriptActor()
{
}

UFieldManager* APGGameLevelScriptActor::GetFieldManager() const
{
	return FieldManager;
}

void APGGameLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	FieldManager = NewObject<UFieldManager>(this);
	if (FieldManager)
	{
		FieldManager->InitializeFieldManager();
	}

}

