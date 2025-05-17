// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NeuroStrikeGameMode.generated.h"

/**
 * @class ANeuroStrikeGameMode
 * @brief Custom game mode class for the NeuroStrike game.
 *
 * ANeuroStrikeGameMode is derived from AGameModeBase and provides
 * functionality to set up core gameplay rules, settings, and behavior
 * specific to the NeuroStrike game.
 *
 * This class is used to define the default player pawn, game session rules,
 * and other gameplay logic relevant to the game mode.
 *
 * The constructor initializes the game mode, including setting up
 * the default pawn class.
 */
UCLASS(minimalapi)
class ANeuroStrikeGameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	/**
	 * @brief Constructor for the ANeuroStrikeGameMode class.
	 *
	 * This constructor initializes the NeuroStrike game mode by setting up
	 * the default pawn class for player characters. The default pawn is configured
	 * using a Blueprint-defined character found at a specified asset path.
	 *
	 * The constructor ensures that the game mode is properly initialized for gameplay.
	 *
	 * @return An instance of ANeuroStrikeGameMode with configured game mode properties.
	 */
	ANeuroStrikeGameMode();
};
