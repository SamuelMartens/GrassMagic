// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *  Implements framework to create gestures in 
 */
class FGMBaseGesture
{
public:

	enum class EType : int8_t
	{
		// Represents X axis
		Damage = 0,
		// Represents Y axis
		Control = 1,
		// Represents Z axis
		Change = 2,
		// Use in case if type is undefined
		None = 3
	};

	FGMBaseGesture(const FGMBaseGesture&) = delete;
	FGMBaseGesture(FGMBaseGesture&&) = delete;
	FGMBaseGesture& operator=(const FGMBaseGesture&) = delete;
	FGMBaseGesture& operator=(FGMBaseGesture&&) = delete;
	
	virtual ~FGMBaseGesture() {};

	float GetValue(EType Type) const noexcept
	{
		switch (Type)
		{
		case Damage:
			return Base.X;
		case Control:
			return Base.Y;
		case Change:
			return Base.Z
		default:
			return 0.0f;
			break;
		}
	}

protected:

	FGMBaseGesture(float X, float Y, float Z) :Base(X, Y, Z)
	{}

private:

	FVector Base;
};


class FGMDamageGesture : public FGMBaseGesture
{

public:

	FGMDamageGesture& static Inst() noexcept
	{
		static FGMDamageGesture Instance(1.0, 0.5, 0.25);
		return Instance;
	};

	// TODO implement priority getter. Use macros or templates

	FGMDamageGesture(const FGMDamageGesture&) = delete;
	FGMDamageGesture(FGMDamageGesture&&) = delete;
	FGMDamageGesture& operator=(const FGMDamageGesture&) = delete;
	FGMDamageGesture& operator=(FGMDamageGesture&&) = delete;
	~FGMDamageGesture() = default;

private:

	FGMDamageGesture(float DamageVal, float ControlVal, float ChangeVal) :
		FGMBaseGesture(DamageVal, ControlVal, ChangeVal)
	{};
};