// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GMInputHandlerGeneric.h"

#include "GMResourceAcquirer.generated.h"

UCLASS()
class GRASSMAGIC_API UGMResourceAcquirer : public UObject
{
	GENERATED_BODY()


	const static float Acquire_Tick;
	const static float Acquire_Delay;
	const static int Resource_Per_Tick = 1;

public:

	UGMResourceAcquirer() = default;
	~UGMResourceAcquirer() = default;

	void SetGenericInputHandler(FGMInputHandlerGeneric NewGenHandler) { GenHandler = NewGenHandler;}

	void StartAcquire();
	void StopAcquire();

	int GetResources() const {  return Resources; };

private:

	UFUNCTION()
	void OnTickResourceAcquire();

	FTimerHandle TimerHandler_ResourceAcquire;

	int Resources = 0;

	FGMInputHandlerGeneric GenHandler;
};
