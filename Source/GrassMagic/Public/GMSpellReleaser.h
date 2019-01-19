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
	const static float Spawn_Rotation_Limit;
	// Stored in degree
	const static float Max_Spread;

public:

	const static float Focus_Min;
	const static float Focus_Max;

	UGMSpellReleaser();
	~UGMSpellReleaser();

	void Init(FGMInputHandlerGeneric NewGenHandler);
	void SetGenericInputHandler(FGMInputHandlerGeneric NewGenHandler);

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

	TSubclassOf<AGMSpellProjectile> BPProjectileClass;

	FGMInputHandlerGeneric GenHandler;
	// We can actually get this from GenHandler, but since we need it every tick,
	// it makes sense to just cache it
	class UGMSpellComponent* SpellComp;
};
