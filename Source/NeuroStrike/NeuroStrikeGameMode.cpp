// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeuroStrikeGameMode.h"
#include "NeuroStrikeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANeuroStrikeGameMode::ANeuroStrikeGameMode() : Super() {
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
		TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	this->DefaultPawnClass = PlayerPawnClassFinder.Class;
}
