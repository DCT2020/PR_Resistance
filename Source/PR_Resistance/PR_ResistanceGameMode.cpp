// Copyright Epic Games, Inc. All Rights Reserved.

#include "PR_ResistanceGameMode.h"
#include "Chara/PR_ResistanceCharacter.h"
#include "UObject/ConstructorHelpers.h"

APR_ResistanceGameMode::APR_ResistanceGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
