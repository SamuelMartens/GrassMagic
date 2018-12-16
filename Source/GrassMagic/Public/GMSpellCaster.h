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


public: 

	UGMSpellCaster(): CastStartTime(0.0f)
	{}

	void Init(AActor* OwnerActor);

	void StartDamageGesture();
	void StopDamageGesture();

private:

	void StartGestureGeneric();
	void StopGestureGeneric(FVector GestureVector);

	FGMSpellState State;

	TWeakObjectPtr<AActor> Owner;

	FTimerHandle TimerHandle_GestureCast;

	FTimespan CastStartTime;
	
};
