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
		ActiveGrade = 1;
		break;
	case 1:
		Base.Bivector = FGA::WedgeProduct(Base.Vector, Gesture.GetBase());
		ActiveGrade = 2;
		break;
	case 2:
		Base.Trivector = FGA::WedgeProduct(Base.Bivector, Gesture.GetBase());
		ActiveGrade = 3;
	case 3:
		Base.Scalar = 0;
		ActiveGrade = 0;
	default:
		check(false);
		break;
	}
}

void FGMSpellState::UpdateActiveEffect(const FGMBaseGesture& Gesture)
{

}
