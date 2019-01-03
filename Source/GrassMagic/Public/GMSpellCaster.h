// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GMSpellState.h"

#include "GMSpellCaster.generated.h"

UCLASS()
class GRASSMAGIC_API UGMSpellCaster : public UObject
{
	GENERATED_BODY()
	const static float Cast_Time_Scale_Coefficient;

public: 

	UGMSpellCaster(): CastStartTime(0.0f)
	{}

	void Init(AActor* OwnerActor);

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

	TWeakObjectPtr<AActor> Owner;

	FTimespan CastStartTime;

	UFUNCTION()
	void Debug_OnTickInfo() const;

#if !UE_BUILD_SHIPPING

	const static float Debug_Info_Tick_Interval;

	FTimerHandle Debug_TimerHandle_Info;

	FVector Debug_GestureVector;
	FGMBaseGesture::EType Debug_GestureType;
#endif

};
