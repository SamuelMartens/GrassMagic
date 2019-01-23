// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GMSpellState.h"

#include "GMInputHandlerGeneric.h"

#include "GMSpellCaster.generated.h"

UCLASS()
class GRASSMAGIC_API UGMSpellCaster : public UObject
{
	GENERATED_BODY()

	const static float Cast_Time_Scale_Coefficient;

	const static float Cast_Tick_Interval;
	const static float Cast_Delay;

public: 

	UGMSpellCaster(): CastStartTime(0.0f)
	{}

	void SetGenericInputHandler(FGMInputHandlerGeneric NewGenHandler) { GenHandler = NewGenHandler; }

	void StartDamageGesture();
	void StopDamageGesture();

	void StartControlGesture();
	void StopControlGesture();

	void StartChangeGesture();
	void StopChangeGesture();

private:

	void StartGestureGeneric();
	void StopGestureGeneric(const FVector& GestureVector, FGMBaseGesture::EType Type);

	double GetCastDurationMilliseconds() const;

	FGMSpellState State;

	FTimespan CastStartTime;

	FGMInputHandlerGeneric GenHandler;

	UFUNCTION()
	void Debug_OnTickInfo() const;

	UFUNCTION()
	void OnCastTick();

	FTimerHandle TimerHandler_CastTick;
	
	FGMBaseGesture::EType CurrentGestureType;

#if !UE_BUILD_SHIPPING

	const static float Debug_Info_Tick_Interval;

	FTimerHandle Debug_TimerHandler_Info;

	FVector Debug_GestureVector;
#endif

};
