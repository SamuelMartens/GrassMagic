// Fill out your copyright notice in the Description page of Project Settings.

#include "GMSpellState.h"


void FGMSpellState::AddGesture(const FGMBaseGesture& Gesture)
{
	// Not sure about order of multiply here. For now will
	// follow the rule OldValue ^ NewValue. But it might
	// produce negative values so may be changedin future
	switch (ActiveGrade)
	{
	case 0:
		Base.Vector = Gesture.GetBase();
		break;
	case 1:
		Base.Bivector = FGA::WedgeProduct(Base.Vector, Gesture.GetBase());
		break;
	case 2:
		Base.Trivector = FGA::WedgeProduct(Base.Bivector, Gesture.GetBase());
	case 3:
		Base.Scalar = 0;
	default:
		check(false);
		break;
	}

	// General routine for mixing gestures
	if (ActiveGrade < FGA::Dimension)
	{
		++ActiveGrade;
		ActiveType = static_cast<uint8_t>(Gesture.GetDominantType()) |
			static_cast<uint8_t>(ActiveType);
	}
	else
	{
		ActiveGrade = 0;
		ActiveType = static_cast<uint8_t>(FGMBaseGesture::EType::None);
	}
}

void FGMSpellState::UpdateActiveEffect(const FGMBaseGesture& Gesture)
{

}
