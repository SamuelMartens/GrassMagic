// Fill out your copyright notice in the Description page of Project Settings.

#include "GMSpellProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "GMSpellComponent.h"
#include "GMSpellEffectManagerComponent.h"
#include "GMSpellEffect.h"
#include "GMMisc.h"

// Sets default values
AGMSpellProjectile::AGMSpellProjectile()
{
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

	AffectOverlappedActor(OtherActor);

	CollisionEffect->Activate();
	CollisionEffect->OnSystemFinished.AddDynamic(this, &AGMSpellProjectile::OnDeath);
}

void AGMSpellProjectile::SetSpellData(float Value, FGMBaseGesture::EType Type, int Grade)
{
	SpellValue = Value;
	SpellType = Type;
	SpellGrade = Grade;

	FVector ColorVal;

	switch (SpellType)
	{
	case FGMBaseGesture::EType::Damage:
		ColorVal = UGMSpellComponent::Damage_Gesture_Effect_Color;
		break;
	case FGMBaseGesture::EType::Control:
		ColorVal = UGMSpellComponent::Control_Gesture_Effect_Color;
		break;
	case FGMBaseGesture::EType::Change:
		ColorVal = UGMSpellComponent::Change_Gesture_Effect_Color;
		break;
	default:
		check(false);
		break;
	}

	LifeEffect->SetVectorParameter("color", ColorVal);
}

void AGMSpellProjectile::OnDeath(UParticleSystemComponent* PSystem)
{
	Destroy();
}

void AGMSpellProjectile::AffectOverlappedActor(AActor* Actor)
{
	UGMSpellEffectManagerComponent* SpellEffectManag = GMMisc::GetCompByClass<UGMSpellEffectManagerComponent>(Actor);

	if (!SpellEffectManag)
		return;

	UGMBaseSpellEffect* SpellEffect = nullptr;

	switch (SpellType)
	{
	case FGMBaseGesture::EType::Damage:
	{
		switch (SpellGrade)
		{
		case 1:
			SpellEffect = NewObject<UGMSpellEffect_Damage_1>(this, UGMSpellEffect_Damage_1::StaticClass());
			break;
		case 2:
			SpellEffect = NewObject<UGMSpellEffect_Damage_2>(this, UGMSpellEffect_Damage_2::StaticClass());
			break;
		case 3:
			SpellEffect = NewObject<UGMSpellEffect_Damage_3>(this, UGMSpellEffect_Damage_3::StaticClass());
			break;
		default:
			check(false);
			break;
		}
	}
	break;
	case FGMBaseGesture::EType::Control:
	{
		switch (SpellGrade)
		{
		case 1:
			SpellEffect = NewObject<UGMSpellEffect_Control_1>(this, UGMSpellEffect_Control_1::StaticClass());
			Cast<UGMSpellEffect_Control_1>(SpellEffect)->SetProjectileImpactPosition(GetActorLocation());
			Cast<UGMSpellEffect_Control_1>(SpellEffect)->SetProjectileImpactRotation(GetActorForwardVector().Rotation());
			break;
		case 2:
			SpellEffect = NewObject<UGMSpellEffect_Control_2>(this, UGMSpellEffect_Control_2::StaticClass());
			break;
		case 3:
			SpellEffect = NewObject<UGMSpellEffect_Control_3>(this, UGMSpellEffect_Control_3::StaticClass());
			break;
		default:
			check(false);
			break;
		}
	}
	break;
	case FGMBaseGesture::EType::Change:
	{
		switch (SpellGrade)
		{
		case 1:
			SpellEffect = NewObject<UGMSpellEffect_Change_1>(this, UGMSpellEffect_Change_1::StaticClass());
			break;
		case 2:
			SpellEffect = NewObject<UGMSpellEffect_Change_2>(this, UGMSpellEffect_Change_2::StaticClass());
			break;
		case 3:
			SpellEffect = NewObject<UGMSpellEffect_Change_3>(this, UGMSpellEffect_Change_3::StaticClass());
			break;
		default:
			check(false);
			break;
		}
	}
	break;

	default:
		check(false);
		break;
	}


	SpellEffect->Init(Cast<APawn>(Actor), Instigator, SpellValue);
	SpellEffectManag->AddAndStartEffect(SpellEffect);
}
