// Copyright Epic Games, Inc. All Rights Reserved.


#include "NeuroStrikePlayerController.h"
#include "EnhancedInputSubsystems.h"

void ANeuroStrikePlayerController::BeginPlay() {
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		this->GetLocalPlayer())) {
		Subsystem->AddMappingContext(InputMappingContext, 0);

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}
}
