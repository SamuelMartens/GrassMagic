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

	enum class EStateParam : uint8_t
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
		ActiveTypes(static_cast<uint8_t>(FGMBaseGesture::EType::None))
	{};

	~FGMSpellState() = default;
	
	void AddEffect(const FVector& EffectValue, FGMBaseGesture::EType Type) noexcept;


	std::tuple<FStateBase, int> GetState() const noexcept 
	{ 
		return std::make_tuple(Base, GetActiveGrade());
	}

	int GetActiveGrade() const noexcept
	{
		int ActiveGrade = 0;

		// ActiveTypes is a bitmask of active types. Our active grade is
		// basically how much active types we have currently
		for (size_t i = 0; i < sizeof(decltype(ActiveTypes)) * 8; ++i)
			ActiveGrade += ((1 << i) & ActiveTypes) ? 1 : 0;
		
		return ActiveGrade;
	}
	
	void Debug_PrintState() const;

private:

	FStateBase Base;
	
	uint8_t ActiveTypes;

};
