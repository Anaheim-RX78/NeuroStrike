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

	/**
	 * Updates the character's state every frame, handling stamina regeneration and debug information display.
	 *
	 * This function is called every tick and performs operations to increment the character's stamina
	 * over time, ensuring it does not exceed the maximum stamina value. It also displays debug messages
	 * with health information for the authoritative player on the screen during gameplay.
	 *
	 * @param DeltaSeconds The time in seconds that has elapsed since the last tick.
	 */
	virtual void Tick(float DeltaSeconds) override;

	/**
	 * Handles the character despawning process in the NeuroStrike game.
	 *
	 * This method broadcasts visual or audio effects to all clients using multicast, provides debug feedback,
	 * and destroys the character instance from the game world.
	 */
	void Despawn();

	/**
	 * Handles the server-side logic for despawning an entity or object in the game.
	 *
	 * This function is marked as reliable and is executed on the server. It is intended
	 * to ensure proper synchronization and management of game state related to despawning
	 * entities during networked gameplay.
	 */
	UFUNCTION(Server, Reliable)
	void ServerDespawn();

	/**
	 * Reduces the character's stamina by a specified amount, typically when performing stamina-consuming actions.
	 *
	 * This function decreases the character's base stamina based on the provided stamina cost value. It is used to manage
	 * stamina depletion mechanics for gameplay elements such as sprinting, dodging, or using abilities.
	 *
	 * @param StaminaCost The amount of stamina to subtract from the character's current stamina.
	 */
	void DecreaseStamina(float StaminaCost);

	/**
	 * Decreases the character's health by the specified amount and handles death mechanics when health reaches zero.
	 *
	 * This method reduces the health of the character and checks whether the health value has fallen to or below zero.
	 * If the character's health is depleted, it triggers necessary visual effects and destroys the character object.
	 * Authority checks ensure that only the server can modify health and trigger related mechanics.
	 *
	 * @param HealthCost The amount of damage to reduce from the character's current health.
	 */
	void DecreaseHealth(float HealthCost);

	/**
	 * Handles the process of decreasing the character's health, ensuring proper client-server communication in a networked environment.
	 *
	 * If called on the server, directly decreases the health by invoking the internal function.
	 * If called on the client, sends a request to the server to process the health decrease.
	 *
	 * @param HealthCost The amount of health to be deducted from the character.
	 */
	void DecreaseHealthHandler(float HealthCost);

	/**
	 * Decreases the character's health on the server with a specified cost.
	 *
	 * This function is marked to run on the server and ensures reliable execution.
	 * It allows the reduction of health in a networked environment ensuring proper
	 * synchronization across clients.
	 *
	 * @param HealthCost The amount of health to decrease.
	 */
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

	/**
	 * Defines the maximum health value for the character.
	 *
	 * This variable is configurable and represents the upper limit of the character's health,
	 * determining the maximum amount of damage the character can sustain before death or incapacitation.
	 */
	UPROPERTY(VisibleAnywhere)
	float MaxHealth = 100.0f;

	/**
	 * Represents the health value of the character, indicating their current vitality in the game.
	 *
	 * This variable is replicated to ensure synchronization of the character's health status across the network
	 * in multiplayer gameplay. Changes to this value reflect damage taken or health recovery during gameplay.
	 */
	UPROPERTY(Replicated)
	float Health;

	/**
	 * Executes visual effects for character despawning on all connected clients.
	 *
	 * This function is called using the NetMulticast specifier, ensuring that the despawn effects are
	 * replicated across the server and all clients reliably. These effects may include visual particles,
	 * sound cues, or other representations associated with the character's disappearance from the game world.
	 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDespawnEffects();

	/**
	 * Configurable static mesh representing the tomb associated with the player.
	 *
	 * This property allows customization of the tomb mesh in the editor under the "Player" category.
	 * It can be set to any compatible static mesh asset to visually represent the player's tomb.
	 */
	UPROPERTY(EditAnywhere, Category="Player")
	UStaticMesh* TombMesh;

	UFUNCTION(Server, Reliable)
	void ServerPrintGameOverIfNoActors();

	void PrintGameOverIfNoActors();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
