// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsComponent.h"

UStatsComponent::UStatsComponent() {
	this->PrimaryComponentTick.bCanEverTick = true;
}

bool UStatsComponent::PlayerHasEnoughStamina(float StaminaCost) {
	return this->BaseStamina >= StaminaCost;
}

void UStatsComponent::BeginPlay() {
	Super::BeginPlay();

	this->Health = this->MaxHealth;
	this->BaseStamina = this->MaxStamina;
}

void UStatsComponent::DecreaseStamina(float StaminaCost) {
	this->BaseStamina -= StaminaCost;
}

void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                    FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
