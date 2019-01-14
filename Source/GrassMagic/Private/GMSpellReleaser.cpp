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
	check(Owner.IsValid() && BPProjectileClass);

	IsReleasingCurrently = false;

	FVector CameraLocation;
	FRotator CameraRotation;

	Cast<APlayerController>(Cast<APawn>(Owner)->GetController())->PlayerCameraManager->GetCameraViewPoint(CameraLocation, CameraRotation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Cast<APawn>(Owner);
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Owner->GetWorld()->SpawnActor<AGMSpellProjectile>(BPProjectileClass, CameraLocation, CameraRotation, SpawnParams);
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
