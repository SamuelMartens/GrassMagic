// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GMInputHandlerGeneric.h"
#include "GMGestures.h"

#include "GMResourceAcquirer.generated.h"

UENUM(BlueprintType)
enum class EResourceRestoreStatus : uint8
{
	SelfRestore UMETA(DisplayName = "Self Restore"),
	QuickRestore UMETA(DisplayName = "Quick Restore"),
	SlowRestore UMETA(DisplayName = "Slow Restore")
};

UCLASS()
class GRASSMAGIC_API UGMResourceAcquirer : public UObject
{
	GENERATED_BODY()


	const static float Acquire_Tick;
	const static int Resource_Per_Tick = 1;

	// Regulates how fast amount of resource that we have will reach amount of Envirounment_Resource
	const static float Resource_Balance_Coefficient;
	// Amount of resource around us. Amount of character's resource will eventually reach this value
	const static float Environment_Resource;
	// We drop amount of our resource on this value, so we can quickly acquire more energy
	const static float Acquire_Drop;
	const static float Maximum_Resources;



	const static float Damage_Resource_Per_Tick;
	const static float Control_Resource_Per_Tick;
	const static float Change_Resource_Per_Tick;

public:

	UGMResourceAcquirer() = default;
	~UGMResourceAcquirer();

	void Init(FGMInputHandlerGeneric NewGenHandler);
	void SetGenericInputHandler(FGMInputHandlerGeneric NewGenHandler) { GenHandler = NewGenHandler;}

	void StartAcquire();
	void StopAcquire();

	float GetResources() const {  return Resources; };
	
	static float GetMaximumResources() { return Maximum_Resources; }

	float GetResourcesPercent() const;

	EResourceRestoreStatus GetResourceRestoreStatus() const;

	bool GestureCastTick(FGMBaseGesture::EType GestureType) noexcept;

	void Debug_PrintResource() const;

private:

	UFUNCTION()
	void OnTickResourceAcquire();

	FTimerHandle TimerHandler_ResourceAcquire;

	float Resources = Environment_Resource;

	FGMInputHandlerGeneric GenHandler;
};
