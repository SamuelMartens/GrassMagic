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

	void Init(AActor* OwnerActor);

	void StartDamageGesture();
	void StopDamageGesture();

private:

	UFUNCTION()
	void OnTickGestureCast();

	FGMSpellState State;

	TWeakObjectPtr<AActor> Owner;

	FTimerHandle TimerHandle_GestureCast;

	FVector CurrentCastGesture;
	
};
