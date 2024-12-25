// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLevel/PGGameLevelScriptActor.h"
#include "Field/PGField.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "PortGame/PortGame.h"
#include "Field/FieldManager.h"
#include "Field/ObjectPoolManager.h"
#include "WorldPartition/WorldPartitionSubsystem.h"
#include "WorldPartition/WorldPartition.h"
#include "GameFramework/PlayerStart.h"

APGGameLevelScriptActor::APGGameLevelScriptActor()
{
}

UFieldManager* APGGameLevelScriptActor::GetFieldManager() const
{
	return FieldManager;
}

UObjectPoolManager* APGGameLevelScriptActor::GetObjectPoolManager() const
{
	return ObjectPoolManager;
}

void APGGameLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

   

	ObjectPoolManager = NewObject<UObjectPoolManager>(this);

	if (ObjectPoolManager)
	{
		ObjectPoolManager->InitializePool();

	}
	FieldManager = NewObject<UFieldManager>(this);
	if (FieldManager)
	{
		FieldManager->InitializeFieldManager();
		
	}

}

