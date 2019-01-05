// Fill out your copyright notice in the Description page of Project Settings.

#include "GMSpellReleaser.h"

const float UGMSpellReleaser::Focus_Min = 1.0f;
const float UGMSpellReleaser::Focus_Tick_Interva = 0.005f;
const float UGMSpellReleaser::Focus_Tick_Increment = 0.005f;
const float UGMSpellReleaser::Focus_Tick_Decrement = 0.005f;

UGMSpellReleaser::UGMSpellReleaser():
	Owner(nullptr),
	Focus(Focus_Min),
	IsReleasingCurrently(false)
{}

UGMSpellReleaser::~UGMSpellReleaser()
{
	if (Owner.IsValid())
		Owner->GetWorldTimerManager().ClearTimer(TimerHandler_Release);
}

void UGMSpellReleaser::Init(AActor* OwnerActor)
{
	check(OwnerActor);

	Owner = OwnerActor;
	
	Owner->GetWorldTimerManager().SetTimer(TimerHandler_Release, this, 
		&UGMSpellReleaser::OnTickSpellRelease, Focus_Tick_Interva, true);
}

void UGMSpellReleaser::StartRelease()
{
	IsReleasingCurrently = true;
}

void UGMSpellReleaser::StopRelease()
{
	IsReleasingCurrently = false;
}

void UGMSpellReleaser::OnTickSpellRelease()
{
	if (IsReleasingCurrently)
	{
		Focus += Focus_Tick_Increment;
	}
	else
	{
		Focus = FMath::Max(Focus_Min, Focus - Focus_Tick_Decrement);	
	}
}
