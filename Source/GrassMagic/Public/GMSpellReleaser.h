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

	const static int Aim_Trace_Length = 100000;
	// Offset from character eye location at which we will spawn projectile
	const static float Spawn_Location_Offset;

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

	bool GetAimLocationAndRotation(FVector& Location, FRotator& Rotation);

	FTimerHandle TimerHandler_Release;

	float Focus;

	bool IsReleasingCurrently;
	
	TSubclassOf<AGMSpellProjectile> BPProjectileClass;

	FGMInputHandlerGeneric GenHandler;
};
