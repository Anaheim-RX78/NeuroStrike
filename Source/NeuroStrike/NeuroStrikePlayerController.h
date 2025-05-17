// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NeuroStrikePlayerController.generated.h"

class UInputMappingContext;

/**
 * Player controller class for handling input-related operations specific to NeuroStrike.
 */
UCLASS()
class NEUROSTRIKE_API ANeuroStrikePlayerController : public APlayerController {
	GENERATED_BODY()

protected:
	/**
	 * Reference to an input mapping context used to define and manage input configurations for the player controller.
	 * Allows customization and handling of input mappings in conjunction with enhanced input systems.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	/**
	 * Initializes the player controller when the game starts or when the player is spawned.
	 *
	 * This method performs the following actions:
	 * - Calls the base implementation of BeginPlay from the parent class.
	 * - Retrieves the enhanced input subsystem for the local player and adds the defined input mapping context.
	 * - Logs a debug message indicating the execution of BeginPlay.
	 */
	virtual void BeginPlay() override;
};
