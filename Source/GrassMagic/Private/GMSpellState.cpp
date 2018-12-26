// Fill out your copyright notice in the Description page of Project Settings.

#include "GMSpellState.h"


void FGMSpellState::AddEffect(const FVector& EffectValue, FGMBaseGesture::EType Type) noexcept
{

	int ActiveGrade = GetActiveGrade();

	// Not sure about order of multiply here. For now will
	// follow the rule OldValue ^ NewValue. But it might
	// produce negative values so may be changed in future
	switch (ActiveGrade)
	{
	case 0:
		Base.Vector = EffectValue;
		break;
	case 1:
		Base.Bivector = FGA::WedgeProduct(Base.Vector, EffectValue);
		break;
	case 2:
		Base.Trivector = FGA::WedgeProduct(Base.Bivector, EffectValue);
	case 3:
		// We need to make sure that this may happen only if we reach our dimensions amount.
		// Because that's how Grassmann Algebra works
		check(ActiveGrade == FGA::Dimensions);
		Base.Scalar = 0;
	default:
		check(false);
		break;
	}

	// General routine for mixing gestures
	if (ActiveGrade < FGA::Dimensions)
	{
		ActiveTypes = static_cast<uint8_t>(Type) |
			static_cast<uint8_t>(ActiveTypes);
	}
	else
	{
		ActiveTypes = static_cast<uint8_t>(FGMBaseGesture::EType::None);
	}
}