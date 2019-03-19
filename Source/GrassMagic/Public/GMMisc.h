// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// This Id is mainly for AddOnScreenDebugMessage function used in debug purposes,
// just for unification purposes
#define GM_DEBUG_GESTURE_CAST_ID __LINE__
#define GM_DEBUG_SPELL_STATE_ID __LINE__
#define GM_DEBUG_RESOURCE_NUM_ID __LINE__


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


	template<typename T>
	T* GetCompByClass(AActor* Actor)
	{
		static_assert(std::is_base_of<UActorComponent, T>::value, "Type should be derived from ActorComponent");
		return Cast<T>(Actor->GetComponentByClass(T::StaticClass()));
	}

	template<typename T>
	T* GetCompByClassCheck(AActor* Actor)
	{
		T* GetResult = GetCompByClass<T>(Actor);
		check(GetResult);

		return GetResult;
	}
}