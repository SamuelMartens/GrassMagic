// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// This Id is mainly for AddOnScreenDebugMessage function used in debug purposes,
// just for unification purposes
#define GM_DEBUG_GESTURE_CAST_ID __LINE__
#define GM_DEBUG_SPELL_STATE_ID __LINE__

namespace GMMisc
{
	inline FRotator ClampRotator(const FRotator& OriginalRot, const FRotator& NewRot, float ClampAngle) noexcept
	{
		FRotator DiffRot = NewRot - OriginalRot;

		DiffRot.Yaw = FMath::Clamp(DiffRot.Yaw, -ClampAngle, ClampAngle);
		DiffRot.Pitch = FMath::Clamp(DiffRot.Pitch, -ClampAngle, ClampAngle);
		DiffRot.Roll = FMath::Clamp(DiffRot.Roll, -ClampAngle, ClampAngle);
		
		return OriginalRot + DiffRot;
	}
}