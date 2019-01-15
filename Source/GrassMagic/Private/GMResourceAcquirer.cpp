// Fill out your copyright notice in the Description page of Project Settings.

#include "GMResourceAcquirer.h"

const float UGMResourceAcquirer::Acquire_Tick = 0.15f;
const float UGMResourceAcquirer::Acquire_Delay = 0.35f;

void UGMResourceAcquirer::StartAcquire()
{
	// Set timer to start acquire
	GenHandler.GerOwner()->GetWorldTimerManager().SetTimer(TimerHandler_ResourceAcquire, this,
		&UGMResourceAcquirer::OnTickResourceAcquire, Acquire_Tick, true, Acquire_Delay);
}

void UGMResourceAcquirer::StopAcquire()
{
	GenHandler.GerOwner()->GetWorldTimerManager().ClearTimer(TimerHandler_ResourceAcquire);

	GenHandler.ExecuteReleaseCallBack();
}

void UGMResourceAcquirer::OnTickResourceAcquire()
{
	Resources += Resource_Per_Tick;
}


