// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeuroStrikeProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ANeuroStrikeProjectile::ANeuroStrikeProjectile() {
	this->CollisionComp = this->CreateDefaultSubobject<USphereComponent>("SphereComp");
	this->CollisionComp->InitSphereRadius(5.0f);
	this->CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	this->CollisionComp->OnComponentHit.AddDynamic(this, &ANeuroStrikeProjectile::OnHit);
	this->CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	this->CollisionComp->CanCharacterStepUpOn = ECB_No;

	this->SetRootComponent(this->CollisionComp);

	this->ProjectileMovement = this->CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComp");
	this->ProjectileMovement->UpdatedComponent = this->CollisionComp;
	this->ProjectileMovement->InitialSpeed = 3000.f;
	this->ProjectileMovement->MaxSpeed = 3000.f;
	this->ProjectileMovement->bRotationFollowsVelocity = true;
	this->ProjectileMovement->bShouldBounce = true;

	this->InitialLifeSpan = 3.0f;
}

void ANeuroStrikeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                   FVector NormalImpulse, const FHitResult& Hit) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics()) {
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		this->Destroy();
	}
}
