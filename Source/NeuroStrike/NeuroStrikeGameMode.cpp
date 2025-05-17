// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeuroStrikeGameMode.h"
#include "NeuroStrikeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANeuroStrikeGameMode::ANeuroStrikeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
