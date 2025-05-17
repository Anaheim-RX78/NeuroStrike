// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_PickUpComponent.h"

UTP_PickUpComponent::UTP_PickUpComponent() {
	SphereRadius = 32.f;
}

void UTP_PickUpComponent::BeginPlay() {
	Super::BeginPlay();

	this->OnComponentBeginOverlap.AddDynamic(this, &UTP_PickUpComponent::OnSphereBeginOverlap);
}

void UTP_PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                               const FHitResult& SweepResult) {
	ANeuroStrikeCharacter* Character = Cast<ANeuroStrikeCharacter>(OtherActor);
	if (Character != nullptr) {
		this->OnPickUp.Broadcast(Character);

		this->OnComponentBeginOverlap.RemoveAll(this);
	}
}
