// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrassmannAlgebra.h"

/**
 *  Implements framework to create gestures in 
 */
class FGMBaseGesture
{
public:

	enum class EType : int8_t
	{
		// Represents X axis
		Damage = FGA::EBasis::E1,
		// Represents Y axis
		Control = FGA::EBasis::E2,
		// Represents Z axis
		Change = FGA::EBasis::E3,
		// Use in case if type is undefined
		None = FGA::EBasis::None
	};

	FGMBaseGesture(const FGMBaseGesture&) = delete;
	FGMBaseGesture(FGMBaseGesture&&) = delete;
	FGMBaseGesture& operator=(const FGMBaseGesture&) = delete;
	FGMBaseGesture& operator=(FGMBaseGesture&&) = delete;
	
	virtual ~FGMBaseGesture() {};

	FVector GetBase() const noexcept { return Base; }

	FGMBaseGesture(float X, float Y, float Z) :Base(X, Y, Z)
	{}

	virtual EType GetDominantType() const = 0;

private:

	FVector Base;
};


class FGMDamageGesture : public FGMBaseGesture
{

public:

	const static float Damage_Val;
	const static float Control_Val;
	const static float Change_Val;

	static FGMDamageGesture& Inst() noexcept
	{
		static FGMDamageGesture Instance(Damage_Val, Control_Val, Change_Val);
		return Instance;
	};

	virtual EType GetDominantType() const override { return EType::Damage; }

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


class FGMControlGesture : public FGMBaseGesture
{

public:

	const static float Damage_Val;
	const static float Control_Val;
	const static float Change_Val;

	static FGMControlGesture& Inst() noexcept
	{
		static FGMControlGesture Instance(Damage_Val, Control_Val, Change_Val);
		return Instance;
	};

	virtual EType GetDominantType() const override { return EType::Control; }

	FGMControlGesture(const FGMControlGesture&) = delete;
	FGMControlGesture(FGMControlGesture&&) = delete;
	FGMControlGesture& operator=(const FGMControlGesture&) = delete;
	FGMControlGesture& operator=(FGMControlGesture&&) = delete;
	~FGMControlGesture() = default;

private:

	FGMControlGesture(float DamageVal, float ControlVal, float ChangeVal) :
		FGMBaseGesture(DamageVal, ControlVal, ChangeVal)
	{};
};


class FGMChangeGesture : public FGMBaseGesture
{

public:

	const static float Damage_Val;
	const static float Control_Val;
	const static float Change_Val;

	static FGMChangeGesture& Inst() noexcept
	{
		static FGMChangeGesture Instance(Damage_Val, Control_Val, Change_Val);
		return Instance;
	};

	virtual EType GetDominantType() const override { return EType::Change; }

	FGMChangeGesture(const FGMChangeGesture&) = delete;
	FGMChangeGesture(FGMChangeGesture&&) = delete;
	FGMChangeGesture& operator=(const FGMChangeGesture&) = delete;
	FGMChangeGesture& operator=(FGMChangeGesture&&) = delete;
	~FGMChangeGesture() = default;

private:

	FGMChangeGesture(float DamageVal, float ControlVal, float ChangeVal) :
		FGMBaseGesture(DamageVal, ControlVal, ChangeVal)
	{};
};

