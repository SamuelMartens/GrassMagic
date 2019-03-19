// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GMCharacter.h"

#include "GMAICreature.generated.h"

UCLASS()
class GRASSMAGIC_API AGMAICreature : public AGMCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	class UGMHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpellSystem, meta = (AllowPrivateAccess = "true"))
	class UGMSpellEffectManagerComponent* SpellEffectManagComp;

public:
	// Sets default values for this character's properties
	AGMAICreature();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
