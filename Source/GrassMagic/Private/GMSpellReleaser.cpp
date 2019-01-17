// Fill out your copyright notice in the Description page of Project Settings.

#include "GMSpellReleaser.h"

#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"

#include "GMSpellProjectile.h"
#include "GMSpellComponent.h"

const float UGMSpellReleaser::Focus_Min = 0.0f;
const float UGMSpellReleaser::Focus_Max = 1.5f;
const float UGMSpellReleaser::Focus_Tick_Interva = 0.005f;
const float UGMSpellReleaser::Focus_Tick_Increment = 0.009f;
const float UGMSpellReleaser::Focus_Tick_Decrement = 0.005f;

const float UGMSpellReleaser::Spawn_Location_Offset = 75.0f;

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
	
	GenHandler.GetSpellComp()->CurrentAction = ESpellComponentCurrentAction::Release;
}

void UGMSpellReleaser::SetSpellProjectileBPType(TSubclassOf<AGMSpellProjectile> NewBPProjectileClass)
{
	BPProjectileClass = NewBPProjectileClass;
}

void UGMSpellReleaser::SpawnProjectile()
{
	FVector SpawnLocation;
	FRotator SpawnRotation;

	GetAimLocationAndRotation(SpawnLocation, SpawnRotation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = GenHandler.GerOwner().Get();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GenHandler.GerOwner()->GetWorld()->SpawnActor<AGMSpellProjectile>(BPProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
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

bool UGMSpellReleaser::GetAimLocationAndRotation(FVector& Location, FRotator& Rotation)
{
	check(GenHandler.GetSpellComp()->GetLeftHandCastSocket().IsValid());

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GenHandler.GerOwner().Get());
	QueryParams.bTraceComplex = false;

	FVector CameraLocation;
	FRotator CameraRotation;
	Cast<APlayerController>(GenHandler.GerOwner()->GetController())->PlayerCameraManager->GetCameraViewPoint(CameraLocation, CameraRotation);

	// Get location of left hand socket
	Location = Cast<USkeletalMeshComponent>(GenHandler.GerOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()))->GetSocketLocation(GenHandler.GetSpellComp()->GetLeftHandCastSocket());

	const FVector TraceEnd = CameraLocation + CameraRotation.Vector() * Aim_Trace_Length;

	FHitResult HitResult;

	if (!GenHandler.GerOwner()->GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Vehicle, QueryParams))
	{
		Rotation = CameraRotation;
		return false;
	}

	Rotation = (HitResult.Location - Location).Rotation();
	return true;
}
