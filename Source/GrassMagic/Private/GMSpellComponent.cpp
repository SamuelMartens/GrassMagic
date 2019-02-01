// Fill out your copyright notice in the Description page of Project Settings.
#include "GMSpellComponent.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "GMResourceAcquirer.h"
#include "GMSpellCaster.h"
#include "GMSpellReleaser.h"
#include "GMInputHandlerGeneric.h"

const float UGMSpellComponent::Movement_Adjust_Rate = 0.07f;
const float UGMSpellComponent::Movement_Adjust_Timer_Interval = 1.0f;
const float UGMSpellComponent::Movement_Adjust_Cuttoff = 0.1f;

const FVector UGMSpellComponent::Resource_Acquire_Effect_Scale = FVector(0.1f, 0.1f, 0.16f);
const FVector UGMSpellComponent::Gesture_Effect_Scale = FVector(0.1f);

const FVector UGMSpellComponent::Damage_Gesture_Effect_Color = FVector(0, 255, 0);
const FVector UGMSpellComponent::Control_Gesture_Effect_Color = FVector(255, 0, 0);
const FVector UGMSpellComponent::Change_Gesture_Effect_Color = FVector(0, 0, 255);

const float UGMSpellComponent::Cast_Effects_Delay = 0.15f;
const float UGMSpellComponent::Gesture_Effect_Interval = 0.5f;
const float UGMSpellComponent::Gesture_Effect_Time_Dilation = 0.6f;

const float UGMSpellComponent::Default_Custom_Time_Dilation = 1.0f;

namespace
{
	void ResetCastEffect(UParticleSystemComponent* Effect, const FDetachmentTransformRules& DetachTransformRules, float DefaultTimeDilation)
	{
		Effect->ClearParameter("color");
		Effect->CustomTimeDilation = DefaultTimeDilation;

		// We don't want particles that were already emitted follow hands,
		// that's why we call detach here
		Effect->DetachFromComponent(DetachTransformRules);
		Effect->Deactivate();
	}
}

// Sets default values for this component's properties
UGMSpellComponent::UGMSpellComponent() :
	CurrentActionState(ESpellComponentActionState::Idle),
	CurrentAction(ESpellComponentCurrentAction::None),
	MovementOffset(0.0f),
	MaximumMovmentInput(0.0f)
{
	LeftHandEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LeftHandEffect"));
	LeftHandEffect->bAutoActivate = false;
	RightHandEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RightHandEffect"));
	RightHandEffect->bAutoActivate = false;

}

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
	ResAcq->Init(GenHandler);

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
	HandleInputGeneric_Pressed(SpellReleaser, &UGMSpellReleaser::StartRelease, ESpellComponentCurrentAction::Focus);
}

void UGMSpellComponent::HandleReleaseSpell_Released()
{
	HandleInputGeneric_Released(SpellReleaser, &UGMSpellReleaser::StopRelease, ESpellComponentCurrentAction::Focus);
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

void UGMSpellComponent::SpawnProjectile()
{
	check(SpellReleaser);

	SpellReleaser->SpawnProjectile();
}

void UGMSpellComponent::SpawnResourceAcquireEffect()
{
	GenericSpawnCastEffect(ResourceAcquireEffect, Resource_Acquire_Effect_Scale);
}

void UGMSpellComponent::SpawnGestureEffect(ESpellComponentCurrentAction CurrentEffect)
{
	FVector EffectColor;

	switch (CurrentEffect)
	{
	case ESpellComponentCurrentAction::CastDamageGesture:
		EffectColor = Damage_Gesture_Effect_Color;
		break;
	case ESpellComponentCurrentAction::CastControlGesture:
		EffectColor = Control_Gesture_Effect_Color;
		break;
	case ESpellComponentCurrentAction::CastChangeGesture:
		EffectColor = Change_Gesture_Effect_Color;
		break;
	default:
		check(false);
		break;
	}


	LeftHandEffect->SetVectorParameter(FName("color"), EffectColor);
	RightHandEffect->SetVectorParameter(FName("color"), EffectColor);

	LeftHandEffect->CustomTimeDilation = Gesture_Effect_Time_Dilation;
	RightHandEffect->CustomTimeDilation = Gesture_Effect_Time_Dilation;

	GenericSpawnCastEffect(GesturesEffect, Gesture_Effect_Scale, true, Gesture_Effect_Interval);

}

void UGMSpellComponent::SpawnFocusEffect()
{
	LeftHandEffect->CustomTimeDilation = 2.0f;
	RightHandEffect->CustomTimeDilation = 2.0f;

	GenericSpawnCastEffect(FocusEffect, FVector(0.05f), true, 0.1f);
}

void UGMSpellComponent::StopCastEffect()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandler_CastEffect);

	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, false);

	ResetCastEffect(LeftHandEffect, DetachRules, Default_Custom_Time_Dilation);
	ResetCastEffect(RightHandEffect, DetachRules, Default_Custom_Time_Dilation);
}

void UGMSpellComponent::SpawnCastEffectTick()
{
	LeftHandEffect->Activate(true);
	RightHandEffect->Activate(true);
}

void UGMSpellComponent::GenericSpawnCastEffect(UParticleSystem* Effect, const FVector& Scale, bool ShouldLoop, float Interval)
{
	LeftHandEffect->SetTemplate(Effect);
	RightHandEffect->SetTemplate(Effect);

	USkeletalMeshComponent* SkeletalComp = Cast<USkeletalMeshComponent>(GetOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	check(SkeletalComp);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);

	LeftHandEffect->AttachToComponent(SkeletalComp, AttachmentRules, GetLeftHandCastSocket());
	RightHandEffect->AttachToComponent(SkeletalComp, AttachmentRules, GetRightHandCastSocket());

	// Rotation and location needs to be reset. Cause when we detach with Keep World Position rules
	// this info is stored in RelativePosition/Rotation/Scale, and we will have undesirable offset
	LeftHandEffect->RelativeScale3D = Scale;
	RightHandEffect->RelativeScale3D = Scale;
	LeftHandEffect->RelativeLocation = FVector(0, 0, 0);
	RightHandEffect->RelativeLocation = FVector(0, 0, 0);
	LeftHandEffect->RelativeRotation = FRotator(0);
	RightHandEffect->RelativeRotation = FRotator(0);

	// InRate value doesn't matter since the function should be executed only once
	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandler_CastEffect, this,
		&UGMSpellComponent::SpawnCastEffectTick, Interval, ShouldLoop, Cast_Effects_Delay);
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