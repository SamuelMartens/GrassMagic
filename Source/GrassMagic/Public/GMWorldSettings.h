// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "GMWorldSettings.generated.h"

/**
 * 
 */

class UParticleSystem;

UCLASS()
class GRASSMAGIC_API AGMWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell Effects Particles")
	UParticleSystem* DamageGrade1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell Effects Particles")
	UParticleSystem* DamageGrade2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell Effects Particles")
	UParticleSystem* DamageGrade3;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell Effects Particles")
	UParticleSystem* ChangeGrade1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell Effects Particles")
	UParticleSystem* ChangeGrade2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell Effects Particles")
	UParticleSystem* ChangeGrade3;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell Effects Particles")
	UParticleSystem* ControlGrade1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell Effects Particles")
	UParticleSystem* ControlGrade2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell Effects Particles")
	UParticleSystem* ControlGrade3;
};
