// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NEUROSTRIKE_API UStatsComponent : public UActorComponent {
	GENERATED_BODY()

public:
	/**
	 * Constructor for UStatsComponent.
	 *
	 * Initializes the component and sets it to tick every frame. The ticking behavior can be
	 * toggled by modifying the `PrimaryComponentTick.bCanEverTick` property. This component
	 * is designed for managing player stats such as stamina and its regeneration.
	 */
	UStatsComponent();

	/**
	 * Represents the base stamina value for the player.
	 *
	 * This value determines the starting stamina of the player. It is initialized
	 * to 100.0f by default and remains constant unless modified. Stamina depletion
	 * and regeneration mechanisms depend on this base value to set boundaries for the
	 * player's stamina management.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BaseStamina;

	/**
	 * Maximum stamina value for the player.
	 *
	 * This value signifies the upper limit of the player's stamina. It is set
	 * to 100.0f by default and can be used in conjunction with stamina depletion
	 * and regeneration logic to ensure the player's stamina does not exceed this cap.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxStamina = 100.0f;

	/**
	 * Rate at which stamina regenerates over time.
	 *
	 * This variable defines the amount of stamina replenished per second. It is set to 0.2 by
	 * default, meaning the player's stamina will increase at a steady rate of 0.2 per second
	 * during periods of regeneration. Adjusting this value can control the speed of stamina recovery.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float StaminaRegenRate = 0.2f;

	/**
	 * Determines if the player has enough stamina to perform an action.
	 *
	 * This method checks if the player's current stamina is greater than or equal
	 * to the specified stamina cost required for an action. It is useful for
	 * validating whether a stamina-consuming action can proceed.
	 *
	 * @param StaminaCost The amount of stamina required to perform the action.
	 * @return true if the player's current stamina is greater than or equal to the StaminaCost.
	 *         false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool PlayerHasEnoughStamina(float StaminaCost);

	UPROPERTY(VisibleAnywhere)
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere)
	float Health;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	void DecreaseStamina(float StaminaCost);
};
