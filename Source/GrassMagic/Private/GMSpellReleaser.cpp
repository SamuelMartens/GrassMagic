// Fill out your copyright notice in the Description page of Project Settings.

#include "GMSpellReleaser.h"

#include "DrawDebugHelpers.h"
#include "GMSpellProjectile.h"

const float UGMSpellReleaser::Focus_Min = 0.0f;
const float UGMSpellReleaser::Focus_Max = 1.5f;
const float UGMSpellReleaser::Focus_Tick_Interva = 0.005f;
const float UGMSpellReleaser::Focus_Tick_Increment = 0.009f;
const float UGMSpellReleaser::Focus_Tick_Decrement = 0.005f;

UGMSpellReleaser::UGMSpellReleaser():
	Focus(Focus_Min),
	IsReleasingCurrently(false)
{}

UGMSpellReleaser::~UGMSpellReleaser()
{
	if (GenHandler.GerOwner().IsValid())
		GenHandler.GerOwner()->GetWorldTimerManager().ClearTimer(TimerHandler_Release);
}

void UGMSpellReleaser::Init(FGMInputHandlerGeneric NewGenHandler)
{
	GenHandler = NewGenHandler;
	
	GenHandler.GerOwner()->GetWorldTimerManager().SetTimer(TimerHandler_Release, this, 
		&UGMSpellReleaser::OnTickSpellRelease, Focus_Tick_Interva, true);
}

void UGMSpellReleaser::StartRelease()
{
	IsReleasingCurrently = true;
}

void UGMSpellReleaser::StopRelease()
{
	check(GenHandler.GerOwner().IsValid() && BPProjectileClass);

	IsReleasingCurrently = false;

	FVector CameraLocation;
	FRotator CameraRotation;

	Cast<APlayerController>(GenHandler.GerOwner()->GetController())->PlayerCameraManager->GetCameraViewPoint(CameraLocation, CameraRotation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = GenHandler.GerOwner().Get();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GenHandler.GerOwner()->GetWorld()->SpawnActor<AGMSpellProjectile>(BPProjectileClass, CameraLocation, CameraRotation, SpawnParams);

	GenHandler.ExecuteReleaseCallBack();
}

void UGMSpellReleaser::SetSpellProjectileBPType(TSubclassOf<AGMSpellProjectile> NewBPProjectileClass)
{
	BPProjectileClass = NewBPProjectileClass;
}

void UGMSpellReleaser::OnTickSpellRelease()
{
	if (IsReleasingCurrently)
	{
		Focus += Focus_Tick_Increment;
	}
	else
	{
		Focus -= Focus_Tick_Decrement;
	}

	Focus = FMath::Clamp(Focus, Focus_Min, Focus_Max);
}
