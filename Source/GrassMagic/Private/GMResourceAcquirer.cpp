// Fill out your copyright notice in the Description page of Project Settings.

#include "GMResourceAcquirer.h"

#include "GMSpellComponent.h"
#include "GMMisc.h"
#include "GMContract.h"

#include "Kismet/GameplayStatics.h"

const float UGMResourceAcquirer::Acquire_Tick = 0.1f;

const float UGMResourceAcquirer::Acquire_Drop = 1000.0f;
const float UGMResourceAcquirer::Environment_Resource = 200.0f;
const float UGMResourceAcquirer::Resource_Balance_Coefficient = 0.007f;
const float UGMResourceAcquirer::Maximum_Resources = UGMResourceAcquirer::Acquire_Drop + UGMResourceAcquirer::Environment_Resource;

const float UGMResourceAcquirer::Damage_Resource_Per_Tick = 50.0f;
const float UGMResourceAcquirer::Control_Resource_Per_Tick = 70.0f;
const float UGMResourceAcquirer::Change_Resource_Per_Tick = 30.0f;


UGMResourceAcquirer::~UGMResourceAcquirer()
{
	if (GenHandler.GerOwner().IsValid())
		GenHandler.GerOwner()->GetWorldTimerManager().ClearTimer(TimerHandler_ResourceAcquire);
}

void UGMResourceAcquirer::Init(FGMInputHandlerGeneric NewGenHandler)
{
	SetGenericInputHandler(NewGenHandler);

	// Set timer to start acquire
	GenHandler.GerOwner()->GetWorldTimerManager().SetTimer(TimerHandler_ResourceAcquire, this,
		&UGMResourceAcquirer::OnTickResourceAcquire, Acquire_Tick, true);
}

void UGMResourceAcquirer::StartAcquire()
{}

void UGMResourceAcquirer::StopAcquire()
{
	GenHandler.ExecuteReleaseCallBack();
}

float UGMResourceAcquirer::GetResourcesPercent() const
{
	CONTRACT.Postcondition([&] 
	{
		const float percent = GetResources() / GetMaximumResources();
		return percent <= 1.0f && percent >= 0.0f;
	});

	return GetResources() / GetMaximumResources();
}

EResourceRestoreStatus UGMResourceAcquirer::GetResourceRestoreStatus() const
{
	const float Slow_Restore_Coefficient = 0.6f;

	if (GetResources() < Environment_Resource)
		return EResourceRestoreStatus::SelfRestore;

	if (GetResources() <= GetMaximumResources() * Slow_Restore_Coefficient)
		return EResourceRestoreStatus::QuickRestore;

	return EResourceRestoreStatus::SlowRestore;
}

bool UGMResourceAcquirer::GestureCastTick(FGMBaseGesture::EType GestureType) noexcept
{
	float ResourceModifier = 0.0f;

	switch (GestureType)
	{
	case FGMBaseGesture::EType::Damage:
		ResourceModifier = Damage_Resource_Per_Tick;
		break;
	case FGMBaseGesture::EType::Control:
		ResourceModifier = Control_Resource_Per_Tick;
		break;
	case FGMBaseGesture::EType::Change:
		ResourceModifier = Change_Resource_Per_Tick;
		break;
	default:
		check(false);
		break;
	}

	Resources -= ResourceModifier;

	if (Resources < 0)
	{
		Resources = 0;
		return false;
	}

	return true;
}

void UGMResourceAcquirer::Debug_PrintResource() const
{
#if !UE_BUILD_SHIPPING

	GEngine->AddOnScreenDebugMessage(GM_DEBUG_RESOURCE_NUM_ID, 2.0f, FColor::Black,
		FString::Printf(TEXT("Resource : %s"), TCHAR_TO_WCHAR(*FString::SanitizeFloat(Resources))
		));
#endif
}

void UGMResourceAcquirer::OnTickResourceAcquire()
{
	float ResourceModifier = 0.0f;

	if (GenHandler.GetSpellComp()->GetCurrentAction() == ESpellComponentCurrentAction::AcquireResource)
		ResourceModifier = Acquire_Drop;
	

	Resources += (Environment_Resource - Resources + ResourceModifier) * Resource_Balance_Coefficient;

	Debug_PrintResource();
}


