// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GMInputHandlerGeneric.h"

#include "GMSpellReleaser.generated.h"

/**
 * 
 */
UCLASS()
class GRASSMAGIC_API UGMSpellReleaser : public UObject
{
	GENERATED_BODY()

	const static float Focus_Tick_Interva; 
	const static float Focus_Tick_Increment;
	const static float Focus_Tick_Decrement;

public:

	const static float Focus_Min;
	const static float Focus_Max;

	UGMSpellReleaser();
	~UGMSpellReleaser();

	void Init(FGMInputHandlerGeneric NewGenHandler);
	void SetGenericInputHandler(FGMInputHandlerGeneric NewGenHandler) { GenHandler = NewGenHandler; }

	void StartRelease();
	void StopRelease();
	
	float GetFocus() const noexcept { return Focus; }

	void SetSpellProjectileBPType(TSubclassOf<class AGMSpellProjectile> NewBPProjectileClass);

	void SpawnProjectile();

private:

	UFUNCTION()
	void OnTickSpellRelease();

	FTimerHandle TimerHandler_Release;

	float Focus;

	bool IsReleasingCurrently;
	
	TSubclassOf<AGMSpellProjectile> BPProjectileClass;

	FGMInputHandlerGeneric GenHandler;
};
