// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GMSpellComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRASSMAGIC_API UGMSpellComponent : public UActorComponent
{
	GENERATED_BODY()

	// Use UPROPERTY to avoid garbage collection of this object
	UPROPERTY()
	class UGMResourceAcquirer* ResAcq;

	// Use UPROPERTY to avoid garbage collection of this object
	UPROPERTY()
	class UGMSpellCaster* SpellCaster;

public:	
	// Sets default values for this component's properties
	UGMSpellComponent();
	~UGMSpellComponent() = default;

	void Init(float ExpectedMovementInput);

	/* Resource acquire interface */
	void HandleAcquireResourceInput(EInputEvent Action);

	UFUNCTION(BlueprintCallable, Category = "SpellSystem")
	bool IsAcquireResources() const;

	float AdjustMovementOnResourceAcquire(float Value) const;


	/* Spell cast Interface */
	void HandleDamageGesture(EInputEvent Action);
	void HandleControlGesture(EInputEvent Action);
	void HandleChangeGesture(EInputEvent Action);

protected:

public:	
	
	UFUNCTION(BlueprintCallable, Category = "SpellSystem")
	int GetResources() const;

};
