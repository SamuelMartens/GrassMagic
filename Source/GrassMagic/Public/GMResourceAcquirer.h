// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GMResourceAcquirer.generated.h"

UCLASS()
class GRASSMAGIC_API UGMResourceAcquirer : public UObject
{
	GENERATED_BODY()

	enum class State
	{
		Idle,
		Prepare,
		InProgress
	};

	const static float Acquire_Tick;
	const static float Acquire_Delay;
	const static int Resource_Per_Tick = 1;

	const static float Movement_Adjust_Rate;
	const static float Movement_Adjust_Timer_Interval;
	const static float Movement_Adjust_Cuttoff;

public:

	UGMResourceAcquirer();
	~UGMResourceAcquirer() = default;

	void Init(AActor* OwnerActor, float OwnerMovementExpectedInput);

	void Acquire();
	void StopAcquire();
	bool IsAcquire() const;

	int GetResources() const { return Resources; };

	float AdjustMovement(float Value);

private:

	State CurrentState;

	UFUNCTION()
	void OnTickResourceAcquire();

	float Prepare(float InpurValue);

	float MaximumMovmentInput;

	FTimerHandle TimerHandle_ResourceAcquire;

	int Resources = 0;

	TWeakObjectPtr<AActor> Owner;

	float MovementOffset;
};
