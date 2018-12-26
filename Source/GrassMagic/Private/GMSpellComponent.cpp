// Fill out your copyright notice in the Description page of Project Settings.

#include "GMSpellComponent.h"

#include "GMResourceAcquirer.h"
#include "GMSpellCaster.h"

// This macros is here only to avoid boilerplate code
#define  GM_HANDLE_INPUT_BODY(MethodName, CompName, MethodState)  check(CompName); \
if (Action == IE_Pressed) \
{ \
	State = MethodState; \
	CompName->Start##MethodName();\
} \
else \
{ \
	State = ESpellComponentState::None; \
	CompName->Stop##MethodName(); \
}
	

// Sets default values for this component's properties
UGMSpellComponent::UGMSpellComponent()
{
	ResAcq = NewObject<UGMResourceAcquirer>(this, UGMResourceAcquirer::StaticClass(), TEXT("ResourceAcquierer"));
	SpellCaster = NewObject<UGMSpellCaster>(this, UGMSpellCaster::StaticClass(), TEXT("SpellCaster"));

	State = ESpellComponentState::None;
}

void UGMSpellComponent::Init(float ExpectedMovementInput)
{
	ResAcq->Init(GetOwner(), ExpectedMovementInput);
	SpellCaster->Init(GetOwner());
}

void UGMSpellComponent::HandleAcquireResource(EInputEvent Action)
{
	GM_HANDLE_INPUT_BODY(Acquire, ResAcq, ESpellComponentState::AcquireResource)
}

void UGMSpellComponent::HandleDamageGesture(EInputEvent Action)
{
	GM_HANDLE_INPUT_BODY(DamageGesture, SpellCaster, ESpellComponentState::CastDamageGesture)
}

void UGMSpellComponent::HandleControlGesture(EInputEvent Action)
{

}

void UGMSpellComponent::HandleChangeGesture(EInputEvent Action)
{

}

float UGMSpellComponent::AdjustMovementOnResourceAcquire(float Value) const
{
	return ResAcq->AdjustMovement(Value);
}



int UGMSpellComponent::GetResources() const
{
	check(ResAcq);
	return ResAcq->GetResources();
}
