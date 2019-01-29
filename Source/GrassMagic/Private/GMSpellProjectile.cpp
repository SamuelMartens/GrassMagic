// Fill out your copyright notice in the Description page of Project Settings.

#include "GMSpellProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AGMSpellProjectile::AGMSpellProjectile()
{
	//#DEBUG clen up this actor whe you decide what is not needed. After all, do we need tick and BeginPlay?
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComponent->InitSphereRadius(2.5f);

	SphereComponent->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
	LifeEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LifeEffect"));
	LifeEffect->SetupAttachment(SphereComponent);

	CollisionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CollisionEffect"));
	CollisionEffect->SetupAttachment(SphereComponent);
}

void AGMSpellProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	MovementComponent->SetVelocityInLocalSpace(FVector(0, 0, 0));
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LifeEffect->Deactivate();

	CollisionEffect->Activate();
	CollisionEffect->OnSystemFinished.AddDynamic(this, &AGMSpellProjectile::OnDeath);
}

// Called when the game starts or when spawned
void AGMSpellProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGMSpellProjectile::OnDeath(UParticleSystemComponent* PSystem)
{
	Destroy();
}

// Called every frame
void AGMSpellProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

