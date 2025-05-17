// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeuroStrikeProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

/**
 * Represents a projectile in the NeuroStrike game.
 * This class handles the movement, collision, and behavior of the projectile
 * when it interacts with other objects.
 */
UCLASS(config=Game)
class ANeuroStrikeProjectile : public AActor {
	GENERATED_BODY()

	/**
	 * Serves as the collision component for the projectile.
	 * This is a sphere-shaped collision representation used to detect and handle
	 * interactions with other objects in the game world.
	 */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/**
	 * Governs the movement behavior of the projectile.
	 * This component handles the projectile's velocity, acceleration, bouncing, and other movement-related properties,
	 * ensuring consistent physics-based movement within the game world.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	/**
	 * Constructs an instance of ANeuroStrikeProjectile.
	 * Initializes the projectile's collision component, movement component, and sets default properties such as lifespan,
	 * collision handling, and movement characteristics. This ensures the projectile behaves as expected within the game environment.
	 *
	 * @return A default-initialized object of ANeuroStrikeProjectile.
	 */
	ANeuroStrikeProjectile();

	/**
	 * Handles the collision behavior of the projectile upon hitting another object.
	 * This function is triggered when the projectile interacts with another component,
	 * applying any necessary physics impulse and managing the projectile's lifecycle.
	 *
	 * @param HitComp The primitive component of the projectile that caused the hit event.
	 * @param OtherActor The actor that was hit by the projectile.
	 * @param OtherComp The specific component of the actor that was hit.
	 * @param NormalImpulse The impulse applied to the hit component upon collision.
	 * @param Hit Detailed information about the hit, including impact location and normal.
	 */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);

	/**
	 * Retrieves the collision component of the projectile.
	 * The collision component is used to handle interactions and collisions with other objects in the game world.
	 *
	 * @return A pointer to the USphereComponent that serves as the collision component of the projectile.
	 */
	USphereComponent* GetCollisionComp() const {
		return this->CollisionComp;
	}

	/**
	 * Retrieves the movement component of the projectile.
	 * The movement component manages the behavior of the projectile's movement, including velocity, acceleration, and physics interactions.
	 *
	 * @return A pointer to the UProjectileMovementComponent that handles the projectile's movement.
	 */
	UProjectileMovementComponent* GetProjectileMovement() const {
		return this->ProjectileMovement;
	}
};
