// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_WeaponComponent.generated.h"

class ANeuroStrikeCharacter;

/** Weapon component that handles firing mechanics, projectile spawning, and related effects */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NEUROSTRIKE_API UTP_WeaponComponent : public USkeletalMeshComponent {
	GENERATED_BODY()

public:
	/**
	 * Default constructor for the weapon component.
	 * Initializes default values for weapon properties, such as the muzzle offset.
	 * This configuration ensures proper alignment for spawning projectiles.
	 *
	 * @return A new instance of the UTP_WeaponComponent class.
	 */
	UTP_WeaponComponent();

	/** The class type for the projectile spawned when the weapon fires */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ANeuroStrikeProjectile> ProjectileClass;

	/** Sound effect played when the weapon is fired */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** Animation montage played when the weapon is fired */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/**
	 * Offset applied to the weapon's muzzle location when spawning projectiles.
	 * Typically used to ensure projectiles are spawned in front of the weapon to avoid collision
	 * with the owning actor or nearby objects.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/**
	 * Attaches the weapon component to the specified character, enabling the character
	 * to use the weapon and ensuring proper attachment to the character's mesh.
	 *
	 * @param TargetCharacter The character to which the weapon component will be attached.
	 *                        If the character already possesses a rifle, the operation is aborted.
	 */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(ANeuroStrikeCharacter* TargetCharacter);

	/**
	 * Handles the spawning of a projectile at the weapon's muzzle location.
	 * This method determines the spawn position and rotation based on the character's
	 * camera and applies a defined muzzle offset. If a valid projectile class is set
	 * and the world context exists, the method spawns the projectile with
	 * collision handling specified to avoid unintended overlaps.
	 */
	UFUNCTION()
	void HandleProjectile();

	/**
	 * Handles visual and auditory effects triggered when the weapon is fired.
	 * Plays a firing sound at the character's location if a valid sound effect is set.
	 * Executes a firing animation montage if a valid animation is provided and
	 * the character's anim instance is available.
	 */
	UFUNCTION()
	void HandleProjectileFX();

private:
	/** Reference to the Neuro Strike character currently associated with the weapon component.
	 *  Tracks the owning character and enables interaction between the character and the weapon,
	 *  such as determining attachment and possession status.
	 */
	ANeuroStrikeCharacter* Character;
};
