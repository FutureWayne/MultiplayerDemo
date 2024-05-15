// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArenaGameMode.h"
#include "ArenaCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArenaGameMode::AArenaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
