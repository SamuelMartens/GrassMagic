// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GMSpellComponent.generated.h"


UENUM(BlueprintType)
enum class ESpellComponentState : uint8
{
	/* Misc States */
	None UMETA(DisplayName = "None"),
	/* Resource Acquire States */
	AcquireResource	UMETA(DisplayName = "Acquire Resource"),
	/* Spell Cast States */
	CastDamageGesture UMETA(DisplayName = "Cast Damage Gesture"),
	CastControlGesture UMETA(DisplayName = "Cast Control Gesture"),
	CastChangeGesture UMETA(DisplayName = "Cast Change Gesture")
};



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

	ESpellComponentState State;

public:	

	// Sets default values for this component's properties
	UGMSpellComponent();
	~UGMSpellComponent() = default;

	void Init(float ExpectedMovementInput);

	/* Resource acquire interface */
	void HandleAcquireResource(EInputEvent Action);

	float AdjustMovementOnResourceAcquire(float Value) const;


	/* Spell cast Interface */
	void HandleDamageGesture(EInputEvent Action);
	void HandleControlGesture(EInputEvent Action);
	void HandleChangeGesture(EInputEvent Action);
	
	UFUNCTION(BlueprintCallable, Category = "SpellSystem")
	int GetResources() const;

	UFUNCTION(BlueprintCallable, Category = "SpellSystem")
	ESpellComponentState GetState() const noexcept { return State; };
};
