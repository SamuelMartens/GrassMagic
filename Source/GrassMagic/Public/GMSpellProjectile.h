// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GMGestures.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GMSpellProjectile.generated.h"


UCLASS()
class GRASSMAGIC_API AGMSpellProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AGMSpellProjectile();

	UFUNCTION()
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void SetSpellData(float Value, FGMBaseGesture::EType Type, int Grade);
	
	UFUNCTION()
	void OnDeath(UParticleSystemComponent* PSystem);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	class UParticleSystemComponent* LifeEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	class UParticleSystemComponent* CollisionEffect;

private:

	void AffectOverlappedActor(AActor* Actor);

	float SpellValue = 0.0f;

	FGMBaseGesture::EType SpellType = FGMBaseGesture::EType::None;

	int SpellGrade = 0;

};
