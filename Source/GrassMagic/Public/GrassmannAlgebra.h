// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *  Implementation of Grassmann algebra primitives and 
 *  operations in 3D space
 */


// Bivector is different than regular vector in a lot of senses,
// so I am gonna avoid to use regular FVector to somehow represent 
// Bivector

struct FBivector
{
	FBivector():_E23(0.0f), _E31(0.0f), _E12(0.0f)
	{}

	FBivector(float E23Value, float E31Value, float E12Value): 
		_E23(E23Value),
		_E31(E31Value),
		_E12(E12Value)
	{}

	~FBivector() = default;
	FBivector(const FBivector&) = default;
	FBivector(FBivector&&) = default;
	FBivector& operator=(const FBivector&) = default;
	FBivector& operator=(FBivector&&) = default;

private:

	friend struct FGA;

	float _E23;

	float _E31;

	float _E12;

};

struct FTrivector
{

	FTrivector(): _E123(0.0f)
	{}

	FTrivector(float E123Value): _E123(E123Value)
	{}

	~FTrivector() = default;
	FTrivector(const FTrivector&) = default;
	FTrivector(FTrivector&&) = default;
	FTrivector& operator=(const FTrivector&) = default;
	FTrivector& operator=(FTrivector&&) = default;

private:

	friend struct FGA;

	float _E123;
};

FTrivector operator*(float Num, const FTrivector& T) noexcept;


// Grassmann algebra
struct FGA
{
	// I might want to move it our of namespace, cause name is too long

	// Vector
	static float E1(const FVector& V) noexcept { return V.X; }
	static float E2(const FVector& V) noexcept { return V.Y; }
	static float E3(const FVector& V) noexcept { return V.Z; }

	static void E1(FVector& V, float Val) noexcept { V.X = Val; }
	static void E2(FVector& V, float Val) noexcept { V.Y = Val; }
	static void E3(FVector& V, float Val) noexcept { V.Z = Val; }

	// Bivector
	static float E23(const FBivector& B) noexcept { return B._E23; }
	static float E31(const FBivector& B) noexcept { return B._E31; }
	static float E12(const FBivector& B) noexcept { return B._E12; }

	static void E23(FBivector& B, float Val) noexcept { B._E23 = Val; }
	static void E31(FBivector& B, float Val) noexcept { B._E31 = Val; }
	static void E12(FBivector& B, float Val) noexcept { B._E12 = Val; }
	
	// Trivector
	static float E123(const FTrivector& T) noexcept { return T._E123; }
	
	static void E123(FTrivector& T, float Val) noexcept { T._E123 = Val; }


	// Wedge product 
	
	static FBivector WedgeProduct(const FVector& V1, const FVector& V2) noexcept
	{
		return FBivector
		(
			E2(V1) * E3(V2) - E3(V1) * E2(V2),
			E3(V1) * E1(V2) - E1(V1) * E3(V2),
			E1(V1) * E2(V2) - E2(V1) * E1(V2)
		);
	}

	static FTrivector WedgeProduct(const FVector& V, const FBivector& B) noexcept
	{
		return FTrivector
		(
			E1(V) * E23(B) + E2(V) * E31(B) + E3(V) * E12(B)
		);
	}

	static FTrivector WedgeProduct(const FBivector& B, const FVector& V) noexcept
	{
		return -1.0f * WedgeProduct(V, B);
	}
};

// Operators overload
inline FBivector operator*(float Num, const FBivector& B) noexcept
{
	return FBivector
	(
		FGA::E23(B) * Num,
		FGA::E31(B) * Num,
		FGA::E12(B) * Num
	);
}

inline FBivector operator*(const FBivector& B, float Num) noexcept
{
	return Num * B;
}


inline FTrivector operator*(float Num, const FTrivector& T) noexcept
{
	return FTrivector(FGA::E123(T) * Num);
}

inline FTrivector operator*(const FTrivector& T, float Num) noexcept
{
	return Num * T;
}
