// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GMSpellComponent.generated.h"

DECLARE_DELEGATE(DelVoidVoid)


UENUM(BlueprintType)
enum class ESpellComponentCurrentAction : uint8
{
	/* Misc States */
	None UMETA(DisplayName = "None"),
	/* Resource Acquire States */
	AcquireResource	UMETA(DisplayName = "Acquire Resource"),
	/* Spell Cast States */
	CastDamageGesture UMETA(DisplayName = "Cast Damage Gesture"),
	CastControlGesture UMETA(DisplayName = "Cast Control Gesture"),
	CastChangeGesture UMETA(DisplayName = "Cast Change Gesture"),
	/* Spell Release States */
	Release UMETA(DisplayName = "Release Spell")
};

UENUM(BlueprintType)
enum class ESpellComponentActionState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Prepare UMETA(DisplayName = "Prepare"),
	InProgress UMETA(DisplayName = "In Progress")
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRASSMAGIC_API UGMSpellComponent : public UActorComponent
{
	GENERATED_BODY()

	const static float Movement_Adjust_Rate;
	const static float Movement_Adjust_Timer_Interval;
	const static float Movement_Adjust_Cuttoff;


public:	

	// Sets default values for this component's properties
	UGMSpellComponent();
	~UGMSpellComponent() = default;

	void BeginPlay() override;

	void Init(float ExpectedMovementInput);
	float AdjustMovement(float Value);


	/* Resource acquire interface */
	void HandleAcquireResource(EInputEvent Action);


	/* Spell cast Interface */
	void HandleDamageGesture(EInputEvent Action);
	void HandleControlGesture(EInputEvent Action);
	void HandleChangeGesture(EInputEvent Action);

	/* Spell release interface */
	void HandleReleaseSpell(EInputEvent Action);
	
	UFUNCTION(BlueprintCallable, Category = "Spell System")
	int GetResources() const;

	UFUNCTION(BlueprintCallable, Category = "Spell System")
	ESpellComponentCurrentAction GetCurrentAction() const noexcept { return CurrentAction; }

	UFUNCTION(BlueprintCallable, Category = "Spell System")
	ESpellComponentActionState GetCurrentActionState() const noexcept { return CurrentActionState; }

	UFUNCTION(BlueprintCallable, Category = "Spell System")
	float GetFocus() const;

	UFUNCTION(BlueprintCallable, Category = "Spell System")
	float GetFocusMin() const;

	UFUNCTION(BlueprintCallable, Category = "Spell System")
	float GetFocusMax() const;

	UFUNCTION(BlueprintCallable, Category = "Spell System")
	void SetSpellProjectileBPType(TSubclassOf<AGMSpellProjectile> BPProjectileClass);

private:

	template<typename T>
	void HandleInputGeneric(EInputEvent InputAction, T* Component, 
		void (T::*CallBackStart)(), void (T::*CallBackStop)(), ESpellComponentCurrentAction ComponentAction)
	{
		check(Component);

		// We should not do few action in the same time. That's why we check current action state
		if (InputAction == IE_Pressed && CurrentActionState == ESpellComponentActionState::Idle)
		{
			CurrentActionState = ESpellComponentActionState::Prepare;
			CurrentAction = ComponentAction;
			PendingAction.BindUObject(Component, CallBackStart);
		}
		// We react only on release if the same action that currently in progress got release command
		else if (InputAction == IE_Released && ComponentAction == CurrentAction)
		{
			// At this point we should always some action in progress
			check(CurrentActionState != ESpellComponentActionState::Idle);

			CurrentActionState = ESpellComponentActionState::Idle;
			CurrentAction = ESpellComponentCurrentAction::None;
			MovementOffset = 0.0f;
			// Call end of input callback. God, please forgive me this ugly syntax
			(Component->*(CallBackStop))();
		}
	}

	float Prepare(float InpurValue);

	DelVoidVoid PendingAction;

	// Use UPROPERTY to avoid garbage collection of this object
	UPROPERTY()
	class UGMResourceAcquirer* ResAcq;

	// Use UPROPERTY to avoid garbage collection of this object
	UPROPERTY()
	class UGMSpellCaster* SpellCaster;

	UPROPERTY()
	class UGMSpellReleaser* SpellReleaser;

	//#DEBUG
	/*UPROPERTY(EditDefaultsOnly, Category = "Native Setting", meta=(DisplayName="Blueprint Projectile Class"))
	const TSubclassOf<AGMSpellProjectile> BPProjectileClass;*/

	ESpellComponentCurrentAction CurrentAction;
	ESpellComponentActionState CurrentActionState;

	float MaximumMovmentInput;
	float MovementOffset;
};
