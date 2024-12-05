// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/KismetSystemLibrary.h"

#define WLOG(_log_, ...) UE_LOG(LogTemp,Warning,_log_, ##__VA_ARGS__ )

#define SLOG(_log_, ...) UKismetSystemLibrary::PrintString(GetWorld(),         \
FString::Printf(TEXT("[LOG] ")) + FString::Printf(_log_, ##__VA_ARGS__),       \
	true, true,FLinearColor::Green, 3.0f);

#define TAG_ENEMY	TEXT("Enemy")
#define TAG_PLAYER	TEXT("Player")
#define TAG_NPC		TEXT("NPC")
