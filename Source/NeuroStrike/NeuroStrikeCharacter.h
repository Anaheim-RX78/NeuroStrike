// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "NeuroStrikeCharacter.generated.h"

class AStaticMeshActor;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 * Represents a character in the NeuroStrike game with first-person capabilities, weapon usage, and customizable input actions.
 *
 * This class extends the base ACharacter class and provides functionality such as managing
 * first-person cameras, weapon mechanics, input mappings, and player-specific actions like move,
 * look, fire, and jump. It also supports networked gameplay with server and client handling
 * for specific actions.
 */
UCLASS(config=Game)
class ANeuroStrikeCharacter : public ACharacter {
	GENERATED_BODY()

	/**
	 * Represents the first-person skeletal mesh for the character.
	 *
	 * Serves as the visual representation of the character's arms and weapon in first-person perspective,
	 * ensuring precision and alignment with the player's viewpoint during gameplay.
	 */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/**
	 * Defines the first-person camera component used as the primary viewpoint for the character.
	 *
	 * Provides a first-person perspective, aligning closely with the character's movement and aiming direction,
	 * ensuring an immersive experience and accurate camera behavior in gameplay scenarios.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/**
	 * Represents the default input mapping context utilized by the character for managing input bindings.
	 *
	 * Facilitates the configuration of control schemes and interaction mappings, providing a base input context
	 * for user inputs and device compatibility within the application.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/**
	 * Defines the input action associated with triggering the jump functionality for the character.
	 *
	 * Enables configuration and assignment of input bindings for initiating jumps, allowing for flexible
	 * customization and compatibility with various input devices through the editor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/**
	 * Represents the input action associated with controlling the player's movement.
	 *
	 * This property allows for the configuration and mapping of input bindings for player locomotion,
	 * facilitating customization and support for various input devices through the editor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/**
	 * Represents the input action associated with controlling the player's look direction.
	 *
	 * This property enables the mapping and configuration of input bindings for camera or view adjustments,
	 * providing flexibility in the input system and supporting customization through the editor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/**
	 * Represents the input action associated with the player's ability to fire a weapon.
	 *
	 * This property allows designers to define and reference the input binding for firing actions,
	 * facilitating customization and flexibility in the input system through the editor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	/**
	 * Represents the input action used for triggering the sprint functionality in the game.
	 *
	 * This variable allows binding to a defined input within the project settings, enabling
	 * character movement to transition into a sprint state when activated. It can be edited
	 * in the editor, is accessible in Blueprint, and is categorized under Input settings.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

protected:
	/**
	 * Handles initialization logic when the game starts or when the actor is spawned.
	 *
	 * This method is called once at the beginning of the game's lifecycle for this actor.
	 * It performs setup operations like adding the default input mapping context
	 * to the Enhanced Input system for the player's controller.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * Constructs an instance of ANeuroStrikeCharacter with default settings.
	 *
	 * This constructor initializes the character's components and properties, including the collision capsule,
	 * first-person camera, and skeletal mesh for the first-person perspective. It also sets the initial state
	 * of the character, such as not having a rifle.
	 *
	 * @return An initialized ANeuroStrikeCharacter instance with default configurations and components attached.
	 */
	ANeuroStrikeCharacter();

	/**
	 * Represents the weapon functionality for the character.
	 *
	 * This component is responsible for managing weapon-related behaviors and interactions,
	 * such as equipping, firing, or handling weapon-specific mechanics within the character.
	 */
	UPROPERTY()
	class UTP_WeaponComponent* WeaponComponent;

	/**
	 * Indicates whether the character currently possesses a rifle.
	 *
	 * This property holds a boolean value reflecting the character’s rifle possession status.
	 * It is visible in the editor and can be accessed in Blueprints.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/**
	 * Updates the character's rifle possession status.
	 *
	 * This method sets the value of the internal `bHasRifle` variable,
	 * indicating whether the character currently possesses a rifle.
	 *
	 * @param bNewHasRifle A boolean value that determines the new rifle possession status.
	 *                     Set to true if the character should have a rifle, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/**
	 * Checks if the character currently possesses a rifle.
	 *
	 * This function returns the status of whether the character is equipped with a rifle,
	 * which is determined by the value of the internal `bHasRifle` variable.
	 *
	 * @return A boolean value indicating whether the character has a rifle (true if equipped, false otherwise).
	 */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	/**
	 * Initiates the server-side logic for firing a weapon.
	 *
	 * This method is called to ensure that the firing action is handled with server authority.
	 * It is designed for replication and ensures consistency across the network
	 * by propagating the firing request from the client to the server.
	 */
	UFUNCTION(Server, Reliable)
	void ServerFire();

	/**
	 * Triggers the firing visual effects for the character.
	 *
	 * This method is executed across all clients and is intended to display the visual effects
	 * associated with firing a weapon. It does not handle the actual shooting logic or server authority,
	 * focusing solely on client-side effects such as muzzle flashes or particle animations.
	 */
	UFUNCTION(NetMulticast, Unreliable)
	void FireFX();

	/**
	 * Executes the firing mechanism of the character.
	 *
	 * This method triggers the shooting action, which includes handling projectiles through
	 * the weapon component and activating associated visual or auditory effects.
	 * It ensures the weapon component is valid before executing the shooting logic.
	 */
	UFUNCTION()
	void Shoot();

protected:
	/**
	 * Handles the movement action triggered by player input.
	 *
	 * This method processes the 2D input value to determine the movement direction
	 * and applies it to the character's forward and right vectors, resulting in
	 * character movement based on the player's input.
	 *
	 * @param Value The input value representing the 2D vector for movement direction
	 *              (X for right/left and Y for forward/backward movement).
	 */
	void Move(const FInputActionValue& Value);

	/**
	 * Handles the looking action triggered by player input.
	 *
	 * This method processes the input value to adjust the yaw and pitch of the character's controller,
	 * allowing for rotational camera movement in response to player input.
	 *
	 * @param Value The input value representing the 2D vector for camera rotation (yaw and pitch).
	 */
	void Look(const FInputActionValue& Value);

	/**
	 * Handles the firing action triggered by player input.
	 *
	 * This method determines if the character is the server authority and either performs a local shoot action
	 * or calls a server-side firing function for network replication.
	 *
	 * @param InputActionValue Represents the input value associated with the firing action.
	 */
	void Fire(const FInputActionValue& InputActionValue);

	/**
	 * Defines the movement speed of the character while walking.
	 *
	 * This property determines the walking speed in units per second and is used to configure
	 * the character's pace during standard movement. It can be modified or referenced to handle
	 * movement mechanics or gameplay-related features tied to walking behavior. Accessible in
	 * the Movement category for inspection and tuning.
	 */
	UPROPERTY(VisibleAnywhere, Category="Movement")
	float WalkingSpeed = 500.0f;

	/**
	 * Defines the sprinting speed for the character in units per second.
	 *
	 * This property determines the movement speed of the character when sprinting. It is part of the
	 * movement category and can be used to adjust or query the character's maximum sprinting speed
	 * during gameplay.
	 */
	UPROPERTY(VisibleAnywhere, Category="Movement")
	float SprintingSpeed = 750.0f;

	/**
	 * Handles the sprinting action for the NeuroStrike character.
	 *
	 * This method is triggered by an input action and adjusts the character's movement speed
	 * to the sprinting speed if the player has sufficient stamina and is currently moving.
	 * If the player doesn't meet these conditions, the sprint action is stopped.
	 *
	 * @param InputActionValue The input value associated with the sprint action, typically
	 * used for detecting whether the sprint command is active.
	 */
	void Sprint(const FInputActionValue& InputActionValue);

	/**
	 * Checks if the player character is currently moving based on their input vector.
	 *
	 * This method determines whether the player's movement input has a significant value,
	 * indicating active movement. A small tolerance is applied to account for negligible input values.
	 *
	 * @return true if the player is moving, otherwise false.
	 */
	bool IsPlayerMoving();

	/**
	 * Stops the sprinting action and resets the character's movement speed to the walking speed.
	 *
	 * This method reverts the character's maximum walk speed to the predefined walking speed,
	 * effectively ending any active sprinting behavior. It ensures the character transitions
	 * back to a normal walking state.
	 */
	void StopSprinting();

	/**
	 * Sets up player input bindings for this character, including actions for movement, looking, and other functionalities.
	 *
	 * @param InputComponent The input component that handles player inputs for this character.
	 */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	int32 PlayerId;

public:
	/**
	 * Retrieves the first-person skeletal mesh component associated with this character.
	 *
	 * @return A pointer to the USkeletalMeshComponent representing the first-person mesh component.
	 */
	USkeletalMeshComponent* GetMesh1P() const {
		return Mesh1P;
	}

	/**
	 * Retrieves the first-person camera component associated with this character.
	 *
	 * @return A pointer to the UCameraComponent representing the first-person camera component.
	 */
	UCameraComponent* GetFirstPersonCameraComponent() const {
		return FirstPersonCameraComponent;
	}

	virtual void Tick(float DeltaSeconds) override;
	void Despawn();

	UFUNCTION(Server, Reliable)
	void ServerDespawn();

	void DecreaseStamina(float StaminaCost);

	void DecreaseHealth(float HealthCost);

	void DecreaseHealthHandler(float HealthCost);

	UFUNCTION(Server, Reliable)
	void ServerDecreaseHealth(float HealthCost);

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

	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float Health;

	UFUNCTION()
	void OnRep_Health();

	// Optional multicast to notify all clients (visual/audio only, no logic)
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDespawnEffects();

	UPROPERTY(EditAnywhere, Category="Player")
	UStaticMesh* TombMesh;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
