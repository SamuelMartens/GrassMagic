// Fill out your copyright notice in the Description page of Project Settings.

#include "GMSpellComponent.h"

#include "GMResourceAcquirer.h"

// Sets default values for this component's properties
UGMSpellComponent::UGMSpellComponent()
{
	ResAcq = NewObject<UGMResourceAcquirer>(this, UGMResourceAcquirer::StaticClass(), TEXT("ResourceAcquierer"));
}

void UGMSpellComponent::Init(float ExpectedMovementInput)
{
	ResAcq->Init(GetOwner(), ExpectedMovementInput);
}

void UGMSpellComponent::HandleAcquireResourceInput(EInputEvent action)
{
	if (action == IE_Pressed)
		ResAcq->Acquire();
	else
		ResAcq->StopAcquire();
}

bool UGMSpellComponent::IsAcquireResources() const
{
	return ResAcq->IsAcquire();
}

float UGMSpellComponent::AdjustMovementOnResourceAcquire(float Value) const
{
	return ResAcq->AdjustMovement(Value);
}

void UGMSpellComponent::HandleDamageGesture(EInputEvent Action)
{

}

void UGMSpellComponent::HandleControlGesture(EInputEvent Action)
{

}

void UGMSpellComponent::HandleChangeGesture(EInputEvent Action)
{

}

int UGMSpellComponent::GetResources() const
{
	return ResAcq->GetResources();
}
