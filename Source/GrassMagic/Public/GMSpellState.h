// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrassmannAlgebra.h"

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
		ActiveGrade(0)
	{};

	~FGMSpellState() = default;


	void SetBase(float Val) noexcept
	{
		ActiveGrade = 0;
		Base.Scalar = Val;
	}

	void SetBase(FVector Val) noexcept
	{
		ActiveGrade = 1;
		Base.Vector = Val;
	}

	void SetBase(FBivector Val) noexcept
	{
		ActiveGrade = 2;
		Base.Bivector = Val;
	}

	void SetBase(FTrivector Val) noexcept
	{
		ActiveGrade = 3;
		Base.Trivector = Val;
	}	
	
	std::tuple<FStateBase, int> GetState() const noexcept 
	{ 
		return std::make_tuple(Base, ActiveGrade);
	}


private:

	FStateBase Base;

	int ActiveGrade;
};
