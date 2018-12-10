// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrassmannAlgebra.h"


/**
 *  Implements simples stack of spells. 
 */
class GRASSMAGIC_API FGMSpellState
{
public:

	FGMSpellState() :
		ActiveGrade(0),
		Base.Scalar(0)
	{};

	~FGMSpellState() = default;


	void SetState(float Val) noexcept
	{
		ActiveGrade = 0;
		Base.Scalar = Val
	}

	void SetState(FVector Val) noexcept
	{
		ActiveGrade = 1;
		Base.Vector = Val;
	}

	void SetState(FBivector Val) noexcept
	{
		ActiveGrade = 2;
		Base.Bivector = Val;
	}

	void SetState(FTrivector Val) noexcept
	{
		ActiveGrade = 3;
		Base.Trivector = Val;
	}

private:

	// Keeps all possible Grassmann algebra primitives in
	// because each of it can represent current spell state
	union FStateBase
	{
		float      Scalar;
		FVector    Vector;
		FBivector  Bivector;
		FTrivector Trivector;
	};

	FStateBase Base;

	int ActiveGrade;
};
