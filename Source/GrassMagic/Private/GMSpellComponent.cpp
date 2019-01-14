// Fill out your copyright notice in the Description page of Project Settings.
#include "GMSpellComponent.h"
//#DEBUG do I need this?
#include "GameFramework/PawnMovementComponent.h"

#include "GMResourceAcquirer.h"
#include "GMSpellCaster.h"
#include "GMSpellReleaser.h"

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
	
	ResAcq->Init(GetOwner());
	SpellCaster->Init(GetOwner());
	SpellReleaser->Init(GetOwner());

	MaximumMovmentInput = ExpectedMovementInput;
}

void UGMSpellComponent::HandleAcquireResource(EInputEvent Action)
{
	HandleInputGeneric(Action, ResAcq, &UGMResourceAcquirer::StartAcquire,
		&UGMResourceAcquirer::StopAcquire, ESpellComponentCurrentAction::AcquireResource);
}

void UGMSpellComponent::HandleDamageGesture(EInputEvent Action)
{
	HandleInputGeneric(Action, SpellCaster, &UGMSpellCaster::StartDamageGesture,
		&UGMSpellCaster::StopDamageGesture, ESpellComponentCurrentAction::CastDamageGesture);
}

void UGMSpellComponent::HandleControlGesture(EInputEvent Action)
{
	HandleInputGeneric(Action, SpellCaster, &UGMSpellCaster::StartControlGesture,
		&UGMSpellCaster::StopControlGesture, ESpellComponentCurrentAction::CastControlGesture);
}

void UGMSpellComponent::HandleChangeGesture(EInputEvent Action)
{
	HandleInputGeneric(Action, SpellCaster, &UGMSpellCaster::StartChangeGesture,
		&UGMSpellCaster::StopChangeGesture, ESpellComponentCurrentAction::CastChangeGesture);
}

void UGMSpellComponent::HandleReleaseSpell(EInputEvent Action)
{
	//#DEBUG
	// This code will draw debug line on spell cast
	//FVector EyeLocation;
	//FRotator EyeRotation;
	//Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController())->PlayerCameraManager->GetCameraViewPoint(EyeLocation, EyeRotation);

	/*
	Before cast check where camera line hit robust object and then from launch point to this point cast projectile
	
	*/

	//FVector CastDir = EyeRotation.Vector();

	//FVector TraceEnd = EyeLocation + CastDir * 10000;

	//DrawDebugLine(GetOwner()->GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.0, 0, 1.0);
	/*FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Cast<APawn> (GetOwner());*/
	// Rewrite this properly
	/*SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetOwner()->GetWorld()->SpawnActor<AGMSpellProjectile>(BPProjectileClass, EyeLocation + 100 * EyeRotation.Vector(), EyeRotation, SpawnParams);*/
	//END

	HandleInputGeneric(Action, SpellReleaser, &UGMSpellReleaser::StartRelease,
		&UGMSpellReleaser::StopRelease, ESpellComponentCurrentAction::Release);
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