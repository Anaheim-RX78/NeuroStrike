// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "NeuroStrikeCharacter.h"
#include "NeuroStrikeProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

UTP_WeaponComponent::UTP_WeaponComponent() {
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UTP_WeaponComponent::Fire() {
	if (Character == nullptr || Character->GetController() == nullptr) {
		return;
	}

	if (this->GetOwnerRole() == ENetRole::ROLE_Authority) {
		this->HandleProjectile();
	} else {
		this->ServerFire();
	}
}

void UTP_WeaponComponent::ServerFire_Implementation() {
	this->HandleProjectile();
}

void UTP_WeaponComponent::AttachWeapon(ANeuroStrikeCharacter* TargetCharacter) {
	this->Character = TargetCharacter;

	if (this->Character == nullptr || this->Character->GetHasRifle()) {
		return;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(this->Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	this->Character->SetHasRifle(true);

	if (APlayerController* PlayerController = Cast<APlayerController>(this->Character->GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(this->FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(
			PlayerController->InputComponent)) {
			EnhancedInputComponent->BindAction(this->FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
		}
	}
}

void UTP_WeaponComponent::HandleProjectile() {
	if (ProjectileClass != nullptr) {
		UWorld* const World = GetWorld();
		if (World != nullptr) {
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// Spawn the projectile at the muzzle
			World->SpawnActor<ANeuroStrikeProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	this->HandleProjectileFX();
}

void UTP_WeaponComponent::HandleProjectileFX_Implementation() {
	if (FireSound != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	if (FireAnimation != nullptr) {
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr) {
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (Character == nullptr) {
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}
