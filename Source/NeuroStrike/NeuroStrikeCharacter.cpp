// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeuroStrikeCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TP_WeaponComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ANeuroStrikeCharacter::ANeuroStrikeCharacter() {
	this->bHasRifle = false;

	this->GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	this->FirstPersonCameraComponent = this->CreateDefaultSubobject<UCameraComponent>("FirstPersonCamera");
	this->FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	this->FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	this->FirstPersonCameraComponent->bUsePawnControlRotation = true;

	this->Mesh1P = this->CreateDefaultSubobject<USkeletalMeshComponent>("CharacterMesh1P");
	this->Mesh1P->SetOnlyOwnerSee(true);
	this->Mesh1P->SetupAttachment(this->FirstPersonCameraComponent);
	this->Mesh1P->bCastDynamicShadow = false;
	this->Mesh1P->CastShadow = false;
	this->Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	this->Stats = this->CreateDefaultSubobject<UStatsComponent>("Stats");
}

void ANeuroStrikeCharacter::BeginPlay() {
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ANeuroStrikeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(this->JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(this->JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(this->MoveAction, ETriggerEvent::Triggered, this,
		                                   &ANeuroStrikeCharacter::Move);

		EnhancedInputComponent->BindAction(this->LookAction, ETriggerEvent::Triggered, this,
		                                   &ANeuroStrikeCharacter::Look);

		EnhancedInputComponent->BindAction(this->FireAction, ETriggerEvent::Started, this,
		                                   &ANeuroStrikeCharacter::Fire);

		EnhancedInputComponent->BindAction(this->SprintAction, ETriggerEvent::Triggered, this,
										   &ANeuroStrikeCharacter::Sprint);

		EnhancedInputComponent->BindAction(this->SprintAction, ETriggerEvent::Completed, this,
										   &ANeuroStrikeCharacter::StopSprinting);
	} else {
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void ANeuroStrikeCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	static float AccumulatedTime = 0.0f;
	AccumulatedTime += DeltaSeconds;
	if (AccumulatedTime >= 0.1f && this->Stats->BaseStamina < this->Stats->MaxStamina) {
		this->Stats->BaseStamina += this->Stats->StaminaRegenRate;

		AccumulatedTime = 0.0f;
	}
}


void ANeuroStrikeCharacter::FireFX_Implementation() {
	this->WeaponComponent->HandleProjectileFX();
}

void ANeuroStrikeCharacter::Shoot() {
	if (this->WeaponComponent == nullptr) {
		return;
	}

	this->WeaponComponent->HandleProjectile();
	this->FireFX();
}

void ANeuroStrikeCharacter::Move(const FInputActionValue& Value) {
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ANeuroStrikeCharacter::Look(const FInputActionValue& Value) {
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ANeuroStrikeCharacter::Fire(const FInputActionValue& InputActionValue) {
	if (this->GetLocalRole() == ROLE_Authority) {
		this->Shoot();
	} else {
		this->ServerFire();
	}
}

void ANeuroStrikeCharacter::Sprint(const FInputActionValue& InputActionValue) {
	float StaminaCost = 0.1f;
	if (this->Stats->PlayerHasEnoughStamina(StaminaCost) && this->IsPlayerMoving()) {
		this->GetCharacterMovement()->MaxWalkSpeed = this->SprintingSpeed;
		this->Stats->DecreaseStamina(StaminaCost);
	} else {
		this->StopSprinting();
	}
}

void ANeuroStrikeCharacter::StopSprinting() {
	this->GetCharacterMovement()->MaxWalkSpeed = this->WalkingSpeed;
}

bool ANeuroStrikeCharacter::IsPlayerMoving() {
	constexpr float Tolerance = 0.001f;
	return !this->GetCharacterMovement()->GetLastInputVector().IsNearlyZero(Tolerance);
}

void ANeuroStrikeCharacter::SetHasRifle(bool bNewHasRifle) {
	bHasRifle = bNewHasRifle;
}

bool ANeuroStrikeCharacter::GetHasRifle() {
	return bHasRifle;
}

void ANeuroStrikeCharacter::ServerFire_Implementation() {
	this->Shoot();
}
