// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortGameGameMode.h"
#include "PortGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

APortGameGameMode::APortGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
