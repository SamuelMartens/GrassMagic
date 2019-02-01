// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GrassMagic.h"

#include "GMSpellComponent.generated.h"

class UGMResourceAcquirer;
class UParticleSystem;
class UParticleSystemComponent;

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
	Focus UMETA(DisplayName = "Focus Spell"),
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

	const static FVector Resource_Acquire_Effect_Scale;
	const static FVector Gesture_Effect_Scale;

	const static FVector Damage_Gesture_Effect_Color;
	const static FVector Control_Gesture_Effect_Color;
	const static FVector Change_Gesture_Effect_Color;

	const static float Cast_Effects_Delay;

	const static float Gesture_Effect_Interval;
	const static float Gesture_Effect_Time_Dilation;

	const static float Default_Custom_Time_Dilation;


	friend class UGMSpellReleaser;

public:	

	// Sets default values for this component's properties
	UGMSpellComponent();
	~UGMSpellComponent() = default;

	void BeginPlay() override;

	void Init(float ExpectedMovementInput);
	float AdjustMovement(float Value);

	// This function must be called in the end of any Input release Sequence
	UFUNCTION(BlueprintCallable, Category = "Spell System")
	void GenericInputRelease();

	/* Resource acquire interface */
	void HandleAcquireResource_Pressed();
	void HandleAcquireResource_Released();
	

	/* Spell cast Interface */
	void HandleDamageGesture_Pressed();
	void HandleDamageGesture_Released();

	void HandleControlGesture_Pressed();
	void HandleControlGesture_Released();

	void HandleChangeGesture_Pressed();
	void HandleChangeGesture_Released();

	/* Spell release interface */
	void HandleReleaseSpell_Pressed();
	void HandleReleaseSpell_Released();
	
	UFUNCTION(BlueprintCallable, Category = "Spell System")
	int GetResources() const;

	UFUNCTION(BlueprintCallable, Category = "Spell System")
	ESpellComponentCurrentAction GetCurrentAction() const noexcept { return CurrentAction; }

	UFUNCTION(BlueprintCallable, Category = "Spell System")
	ESpellComponentActionState GetCurrentActionState() const noexcept { return CurrentActionState; }

	UFUNCTION(BlueprintCallable, Category = "Spell System")
	float GetFocus() const;

	UFUNCTION(BlueprintCallable, Category = "Spell System Constants")
	float GetFocusMin() const;

	UFUNCTION(BlueprintCallable, Category = "Spell System Constants")
	float GetFocusMax() const;

	UFUNCTION(BlueprintCallable, Category = "Spell System Constants")
	static FVector GetResourceAcquireEffectScale() noexcept { return Resource_Acquire_Effect_Scale; };

	UFUNCTION(BlueprintCallable, Category = "Spell System Effects")
	UParticleSystem* GetResourceAcquireEffect() const { return ResourceAcquireEffect; }

	UFUNCTION(BlueprintCallable, Category = "Spell System")
	void SetSpellProjectileBPType(TSubclassOf<AGMSpellProjectile> BPProjectileClass);

	UFUNCTION(BlueprintCallable, Category = "Spell System")
	void SpawnProjectile();

	UFUNCTION(BlueprintCallable, Category = "Spell System Effect")
	void SpawnResourceAcquireEffect();

	UFUNCTION(BlueprintCallable, Category = "Spell System Effect")
	void SpawnGestureEffect(ESpellComponentCurrentAction CurrentEffect);

	UFUNCTION(BlueprintCallable, Category = "Spell System Effect")
	void SpawnFocusEffect();

	UFUNCTION(BlueprintCallable, Category = "Spell System Effects")
	void StopCastEffect();

	UFUNCTION(BlueprintCallable, Category = "Spell System")
	const FName& GetLeftHandCastSocket() const noexcept { return LeftHandCastSocket; }
	UFUNCTION(BlueprintCallable, Category = "Spell System")
	const FName& GetRightHandCastSocket() const noexcept { return RightHandCastSocket; }

	UGMResourceAcquirer* GetResourceAsq() noexcept { return ResAcq; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spell System Effect")
	UParticleSystem* ResourceAcquireEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spell System Effect")
	UParticleSystem* GesturesEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spell System Effect")
	UParticleSystem* FocusEffect;

private:

	template<typename T>
	void HandleInputGeneric_Pressed( T* Component, void (T::*CallBackStart)(), ESpellComponentCurrentAction ComponentAction)
	{
		check(Component);
		
		// We should not do few action in the same time. That's why we check current action state
		if (CurrentActionState != ESpellComponentActionState::Idle)
			return;

		CurrentActionState = ESpellComponentActionState::Prepare;
	
		CurrentAction = ComponentAction;
		PendingAction.BindUObject(Component, CallBackStart);
	}

	template<typename T>
	void HandleInputGeneric_Released(T* Component, void (T::*CallBackStop)(), ESpellComponentCurrentAction ComponentAction)
	{
		check(Component);

		// We react only on release if the same action that currently in progress got release command
		if (ComponentAction != CurrentAction)
			return;

		if (CurrentActionState == ESpellComponentActionState::Prepare)
		{
			// If this happened then it means that player tried to stop the action on the preparation
			// step, which means there gonna be no action at all. We stop our preparations, cancel any pending
			// actions and return to Idle state
			GenericInputRelease();
			return;
		}

		// Call end of input callback. God, please forgive me this ugly syntax
		(Component->*(CallBackStop))();
	}

	// Spawn action that will be taken after some delay
	UFUNCTION()
	void SpawnCastEffectTick();

	void GenericSpawnCastEffect(UParticleSystem* Effect, const FVector& Scale = FVector(1.0), bool ShouldLoop = false, float Interval = 1.0f);

	float Prepare(float InpurValue);

	DelVoidVoid PendingAction;

	// Use UPROPERTY to avoid garbage collection of this object
	UPROPERTY()
	UGMResourceAcquirer* ResAcq;

	// Use UPROPERTY to avoid garbage collection of this object
	UPROPERTY()
	class UGMSpellCaster* SpellCaster;

	UPROPERTY()
	class UGMSpellReleaser* SpellReleaser;

	ESpellComponentCurrentAction CurrentAction;
	ESpellComponentActionState CurrentActionState;

	float MaximumMovmentInput;
	float MovementOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Spell System")
	FName LeftHandCastSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Spell System")
	FName RightHandCastSocket;

	
	UParticleSystemComponent* LeftHandEffect;
	UParticleSystemComponent* RightHandEffect;
	// We mainly need to have delay before cast effect appears
	FTimerHandle TimerHandler_CastEffect;
};
