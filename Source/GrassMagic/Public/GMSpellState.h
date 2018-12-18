// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrassmannAlgebra.h"
#include "GMGestures.h"

#include <tuple>


/**
 *  Implements simples stack of spells. 
 */
class GRASSMAGIC_API FGMSpellState
{
public:

	enum class EStateParam : int8_t
	{
		StateBase = 0,
		ActiveGrade = 1
	};


	// Keeps all possible Grassmann algebra primitives in
	// because each of it can represent current spell state
	union FStateBase
	{
		FStateBase() : Scalar(0), Vector(), Bivector(), Trivector() {}
		~FStateBase() {}

		float      Scalar;
		FVector    Vector;
		FBivector  Bivector;
		FTrivector Trivector;
	};


	FGMSpellState() :
		ActiveGrade(0),
		ActiveType(static_cast<uint8_t>(FGMBaseGesture::EType::None))
	{};

	~FGMSpellState() = default;
	
	void AddGesture(const FGMBaseGesture& Gesture);

	std::tuple<FStateBase, int> GetState() const noexcept 
	{ 
		return std::make_tuple(Base, ActiveGrade);
	}


private:

	void UpdateActiveEffect(const FGMBaseGesture& Gesture);

	FStateBase Base;
	
	int ActiveGrade;
	uint8_t ActiveType;

};
