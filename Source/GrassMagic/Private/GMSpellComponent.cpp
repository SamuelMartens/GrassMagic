// Fill out your copyright notice in the Description page of Project Settings.
#include "GMSpellComponent.h"
//#DEBUG do I need this?
#include "GameFramework/PawnMovementComponent.h"

#include "GMResourceAcquirer.h"
#include "GMSpellCaster.h"
#include "GMSpellReleaser.h"
#include "GMInputHandlerGeneric.h"

const float UGMSpellComponent::Movement_Adjust_Rate = 0.07f;
const float UGMSpellComponent::Movement_Adjust_Timer_Interval = 1.0f;
const float UGMSpellComponent::Movement_Adjust_Cuttoff = 0.1f;


// Sets default values for this component's properties
UGMSpellComponent::UGMSpellComponent() :
	CurrentActionState(ESpellComponentActionState::Idle),
	CurrentAction(ESpellComponentCurrentAction::None),
	MovementOffset(0.0f),
	MaximumMovmentInput(0.0f)
{}

void UGMSpellComponent::BeginPlay()
{
	Super::BeginPlay();

	ResAcq = NewObject<UGMResourceAcquirer>(this, UGMResourceAcquirer::StaticClass(), TEXT("ResourceAcquierer"));
	SpellCaster = NewObject<UGMSpellCaster>(this, UGMSpellCaster::StaticClass(), TEXT("SpellCaster"));
	SpellReleaser = NewObject<UGMSpellReleaser>(this, UGMSpellReleaser::StaticClass(), TEXT("SpellReleaser"));
}

void UGMSpellComponent::Init(float ExpectedMovementInput)
{
	FGMInputHandlerGeneric GenHandler;
	GenHandler.Init(Cast<APawn>(GetOwner()), this, &UGMSpellComponent::GenericInputRelease);

	SpellReleaser->Init(GenHandler);

	ResAcq->SetGenericInputHandler(GenHandler);
	SpellCaster->SetGenericInputHandler(GenHandler);

	MaximumMovmentInput = ExpectedMovementInput;
}

void UGMSpellComponent::HandleAcquireResource_Pressed()
{
	HandleInputGeneric_Pressed(ResAcq, &UGMResourceAcquirer::StartAcquire, ESpellComponentCurrentAction::AcquireResource);
}

void UGMSpellComponent::HandleAcquireResource_Released()
{
	HandleInputGeneric_Released(ResAcq, &UGMResourceAcquirer::StopAcquire, ESpellComponentCurrentAction::AcquireResource);
}

void UGMSpellComponent::HandleDamageGesture_Pressed()
{
	HandleInputGeneric_Pressed(SpellCaster, &UGMSpellCaster::StartDamageGesture, ESpellComponentCurrentAction::CastDamageGesture);
}

void UGMSpellComponent::HandleDamageGesture_Released()
{
	HandleInputGeneric_Released(SpellCaster, &UGMSpellCaster::StopDamageGesture, ESpellComponentCurrentAction::CastDamageGesture);
}

void UGMSpellComponent::HandleControlGesture_Pressed()
{
	HandleInputGeneric_Pressed(SpellCaster, &UGMSpellCaster::StartControlGesture, ESpellComponentCurrentAction::CastControlGesture);
}

void UGMSpellComponent::HandleControlGesture_Released()
{
	HandleInputGeneric_Released(SpellCaster, &UGMSpellCaster::StopControlGesture, ESpellComponentCurrentAction::CastControlGesture);
}

void UGMSpellComponent::HandleChangeGesture_Pressed()
{
	HandleInputGeneric_Pressed(SpellCaster, &UGMSpellCaster::StartChangeGesture, ESpellComponentCurrentAction::CastChangeGesture);
}

void UGMSpellComponent::HandleChangeGesture_Released()
{
	HandleInputGeneric_Released(SpellCaster, &UGMSpellCaster::StopChangeGesture, ESpellComponentCurrentAction::CastChangeGesture);
}

void UGMSpellComponent::HandleReleaseSpell_Pressed()
{
	HandleInputGeneric_Pressed(SpellReleaser, &UGMSpellReleaser::StartRelease, ESpellComponentCurrentAction::Release);
}

void UGMSpellComponent::HandleReleaseSpell_Released()
{
	HandleInputGeneric_Released(SpellReleaser, &UGMSpellReleaser::StopRelease, ESpellComponentCurrentAction::Release);
}

float UGMSpellComponent::AdjustMovement(float Value)
{
	switch (CurrentActionState)
	{
	case ESpellComponentActionState::Idle:
		return Value;
	case ESpellComponentActionState::Prepare:
		return Prepare(Value);
	case ESpellComponentActionState::InProgress:
		return 0.0f;
	default:
		return Value;
	}
}


void UGMSpellComponent::GenericInputRelease()
{
	// At this point we should always some action in progress
	check(CurrentActionState != ESpellComponentActionState::Idle);

	CurrentActionState = ESpellComponentActionState::Idle;
	CurrentAction = ESpellComponentCurrentAction::None;
	MovementOffset = 0.0f;
}

int UGMSpellComponent::GetResources() const
{
	check(ResAcq);
	return ResAcq->GetResources();
}

float UGMSpellComponent::GetFocus() const
{
	check(SpellReleaser);
	return SpellReleaser->GetFocus();
}

float UGMSpellComponent::GetFocusMin() const
{
	return UGMSpellReleaser::Focus_Min;
}

float UGMSpellComponent::GetFocusMax() const
{
	return UGMSpellReleaser::Focus_Max;
}

void UGMSpellComponent::SetSpellProjectileBPType(TSubclassOf<AGMSpellProjectile> BPProjectileClass)
{
	check(SpellReleaser);
	
    SpellReleaser->SetSpellProjectileBPType(BPProjectileClass);
}

float UGMSpellComponent::Prepare(float InputValue)
{
	check(CurrentActionState == ESpellComponentActionState::Prepare);

	APawn* PawnOwner = Cast<APawn>(GetOwner());

	check(PawnOwner);

	float AdjustedValue = 0.0f;

	if (PawnOwner->GetMovementComponent()->GetLastInputVector().IsZero())
		// We stand still, so don't move until we done cast
		MovementOffset = MaximumMovmentInput;
	else if (InputValue != 0.0)
	{
		const float AbsValue = FMath::Abs(InputValue);

		MovementOffset = FMath::FInterpTo(MovementOffset, MaximumMovmentInput, Movement_Adjust_Timer_Interval, Movement_Adjust_Rate);
		AdjustedValue = AbsValue - MovementOffset;

		if (FMath::IsNearlyZero(AdjustedValue, Movement_Adjust_Cuttoff))
		{
			AdjustedValue = 0.0f;
			MovementOffset = MaximumMovmentInput;
		}
	}

	// If we don't move we can cast
	if (MovementOffset >= MaximumMovmentInput)
	{
		CurrentActionState = ESpellComponentActionState::InProgress;

		check(PendingAction.IsBound());
		PendingAction.Execute();
	}

	return  InputValue > 0.0f ? AdjustedValue : -AdjustedValue;

}