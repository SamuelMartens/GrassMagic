// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GMGestures.h"
#include "GrassmannAlgebra.h"

// GMGestires.h relies on this configuration


template<typename T, FGMBaseGesture::EType EffectType1, FGMBaseGesture::EType EffectType2>
struct GMGetEffectValue
{
	float operator()(const T& ActiveEffectBase)
	{
		return 0.0f;
	}
};


// FVector
template<FGMBaseGesture::EType EffectType2>
struct GMGetEffectValue<FVector, FGMBaseGesture::EType::Damage, EffectType2>
{
	float operator()(const FVector& ActiveEffectBase)
	{
		return FGA::E1(ActiveEffectBase);
	}
};


template<FGMBaseGesture::EType EffectType2>
struct GMGetEffectValue<FVector, FGMBaseGesture::EType::Control, EffectType2>
{
	float operator()(const FVector& ActiveEffectBase)
	{
		return FGA::E2(ActiveEffectBase);
	}
};

template<FGMBaseGesture::EType EffectType2>
struct GMGetEffectValue<FVector, FGMBaseGesture::EType::Change, EffectType2>
{
	float operator()(const FVector& ActiveEffectBase)
	{
		return FGA::E3(ActiveEffectBase);
	}
};


// FBivector
template<>
struct GMGetEffectValue<FBivector, FGMBaseGesture::EType::Damage, FGMBaseGesture::EType::Damage>
{
	float operator()(const FBivector& ActiveEffectBase)
	{
		return 0.0f;
	}
};

template<>
struct GMGetEffectValue<FBivector, FGMBaseGesture::EType::Damage, FGMBaseGesture::EType::Control>
{
	float operator()(const FBivector& ActiveEffectBase)
	{
		return FGA::E12(ActiveEffectBase);
	}
};

template<>
struct GMGetEffectValue<FBivector, FGMBaseGesture::EType::Control, FGMBaseGesture::EType::Damage>
{
	float operator()(const FBivector& ActiveEffectBase)
	{
		GMGetEffectValue<FBivector, FGMBaseGesture::EType::Damage, FGMBaseGesture::EType::Control> GetValue;
		return GetValue(ActiveEffectBase);
	}
};

template<>
struct GMGetEffectValue<FBivector, FGMBaseGesture::EType::Damage, FGMBaseGesture::EType::Change>
{
	float operator()(const FBivector& ActiveEffectBase)
	{
		return FGA::E31(ActiveEffectBase);
	}
};

template<>
struct GMGetEffectValue<FBivector, FGMBaseGesture::EType::Change, FGMBaseGesture::EType::Damage>
{
	float operator()(const FBivector& ActiveEffectBase)
	{
		GMGetEffectValue<FBivector, FGMBaseGesture::EType::Damage, FGMBaseGesture::EType::Change> GetValue;
		return GetValue(ActiveEffectBase);
	}
};



template<>
struct GMGetEffectValue<FBivector, FGMBaseGesture::EType::Control, FGMBaseGesture::EType::Control>
{
	float operator()(const FBivector& ActiveEffectBase)
	{
		return 0.0f;
	}
};

// Control + Damage Exists

template<>
struct GMGetEffectValue<FBivector, FGMBaseGesture::EType::Control, FGMBaseGesture::EType::Change>
{
	float operator()(const FBivector& ActiveEffectBase)
	{
		return FGA::E23(ActiveEffectBase);
	}
};

template<>
struct GMGetEffectValue<FBivector, FGMBaseGesture::EType::Change, FGMBaseGesture::EType::Control>
{
	float operator()(const FBivector& ActiveEffectBase)
	{
		GMGetEffectValue<FBivector, FGMBaseGesture::EType::Control, FGMBaseGesture::EType::Change> GetValue;
		return GetValue(ActiveEffectBase);
	}
};


template<>
struct GMGetEffectValue<FBivector, FGMBaseGesture::EType::Change, FGMBaseGesture::EType::Change>
{
	float operator()(const FBivector& ActiveEffectBase)
	{
		return 0.0f;
	}
};


// Change + Damage Exists
// Change + Control Exists 


// FTrivector
template <FGMBaseGesture::EType EffectType1, FGMBaseGesture::EType EffectType2>
struct GMGetEffectValue<FTrivector, EffectType1, EffectType2>
{
	float operator()(const FTrivector& ActiveEffectBase)
	{
		return FGA::E123(ActiveEffectBase);
	}
};
