// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "NeuroStrikeCharacter.h"
#include "TP_PickUpComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, ANeuroStrikeCharacter*, PickUpCharacter);

/**
 * @class UTP_PickUpComponent
 * @brief A blueprintable and component-based class that defines functionality for an item pickup system.
 *
 * This component inherits from USphereComponent and adds additional logic to handle interactions
 * when another actor, such as a player character, overlaps with the sphere collision area.
 * It is designed to be used in Unreal Engine as a gameplay mechanic allowing
 * objects to be picked up.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NEUROSTRIKE_API UTP_PickUpComponent : public USphereComponent {
	GENERATED_BODY()

public:
	/**
	 * @brief Delegate triggered when an item is picked up by an actor.
	 *
	 * This event can be bound to custom functionality in blueprints or code to handle
	 * the interaction logic when an actor interacts with a pickup component.
	 * It is broadcast whenever a valid actor overlaps with the component and completes the pickup action.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPickUp OnPickUp;

	/**
	 * @brief Default constructor for the UTP_PickUpComponent class.
	 *
	 * Initializes the pickup component with default settings, such as
	 * setting the sphere collision radius to a predefined value. Designed
	 * to be used within Unreal Engine for initializing gameplay functionality.
	 *
	 * @return An instance of UTP_PickUpComponent with default configurations applied.
	 */
	UTP_PickUpComponent();

protected:
	/**
	 * @brief Initializes the UTP_PickUpComponent when the game begins or the component is spawned.
	 *
	 * This overridden method from the USphereComponent class is called during the game startup
	 * to set up the component's initial state. It registers the overlap event handler
	 * OnSphereBeginOverlap, enabling custom logic when another actor enters the sphere's collision area.
	 *
	 * @see UActorComponent::BeginPlay
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Handles the logic when another actor begins overlapping with the sphere collision component.
	 *
	 * This function responds to overlap events triggered by the component and checks if the overlapping actor
	 * is of the correct type (e.g., ANeuroStrikeCharacter). If the condition is met, it broadcasts the
	 * OnPickUp event and unregisters further overlap events for this instance.
	 *
	 * @param OverlappedComponent The sphere component that was overlapped.
	 * @param OtherActor The actor that initiated the overlap event.
	 * @param OtherComp The specific component of the overlapping actor that interacted with the sphere.
	 * @param OtherBodyIndex An index associated with the overlapping object, useful in multi-body components.
	 * @param bFromSweep Indicates whether the overlap was caused by a sweep movement.
	 * @param SweepResult Contains additional information about the sweep (valid only if bFromSweep is true).
	 *
	 * @see OnPickUp
	 */
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                          const FHitResult& SweepResult);
};
