// Copyright Epic Games, Inc. All Rights Reserved.

#include "TouchSandboxGameMode.h"
#include "TouchSandboxCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATouchSandboxGameMode::ATouchSandboxGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
