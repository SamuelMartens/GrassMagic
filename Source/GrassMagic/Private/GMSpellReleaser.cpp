// Fill out your copyright notice in the Description page of Project Settings.

#include "GMSpellReleaser.h"

#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"

#include "GMSpellProjectile.h"
#include "GMSpellComponent.h"
#include "GMMisc.h"

const float UGMSpellReleaser::Focus_Min = 0.0f;
// Be careful with this value. If you make it less or bigger than 1.0 then your
// actual maximum spread angle might be bigger or smaller than the one you set up
const float UGMSpellReleaser::Focus_Max = 1.0f;
const float UGMSpellReleaser::Focus_Tick_Interva = 0.005f;
const float UGMSpellReleaser::Focus_Tick_Increment = 0.009f;
const float UGMSpellReleaser::Focus_Tick_Decrement = 0.005f;

const float UGMSpellReleaser::Spawn_Rotation_Limit = 30.0f;
const float UGMSpellReleaser::Max_Spread = 30.0f;

UGMSpellReleaser::UGMSpellReleaser():
	Focus(Focus_Min),
	SpellComp(nullptr)
{}

UGMSpellReleaser::~UGMSpellReleaser()
{
	if (GenHandler.GerOwner().IsValid())
		GenHandler.GerOwner()->GetWorldTimerManager().ClearTimer(TimerHandler_Release);
}

void UGMSpellReleaser::Init(FGMInputHandlerGeneric NewGenHandler)
{
	SetGenericInputHandler(NewGenHandler);

	GenHandler.GerOwner()->GetWorldTimerManager().SetTimer(TimerHandler_Release, this, 
		&UGMSpellReleaser::OnTickSpellRelease, Focus_Tick_Interva, true);
}

void UGMSpellReleaser::StartRelease()
{
}

void UGMSpellReleaser::StopRelease()
{
	check(GenHandler.GerOwner().IsValid() && BPProjectileClass);	
	SpellComp->CurrentAction = ESpellComponentCurrentAction::Release;
}

void UGMSpellReleaser::SetGenericInputHandler(FGMInputHandlerGeneric NewGenHandler)
{ 
	GenHandler = NewGenHandler;
	SpellComp = GenHandler.GetSpellComp();

	check(SpellComp);
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
	switch (SpellComp->GetCurrentAction())
	{
	case ESpellComponentCurrentAction::Focus:
		Focus += Focus_Tick_Increment;
		break;
	case ESpellComponentCurrentAction::Release:
		break;
	default:
		Focus -= Focus_Tick_Decrement;
		break;
	}

	Focus = FMath::Clamp(Focus, Focus_Min, Focus_Max);
}

bool UGMSpellReleaser::GetAimLocationAndRotation(FVector& Location, FRotator& Rotation)
{
	check(SpellComp->GetLeftHandCastSocket().IsValid());

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GenHandler.GerOwner().Get());
	QueryParams.bTraceComplex = false;

	FVector CameraLocation;
	FRotator CameraRotation;
	Cast<APlayerController>(GenHandler.GerOwner()->GetController())->PlayerCameraManager->GetCameraViewPoint(CameraLocation, CameraRotation);

	// Get location of left hand socket
	const USkeletalMeshComponent* SkeletalComp = Cast<USkeletalMeshComponent>(GenHandler.GerOwner()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	Location = SkeletalComp->GetSocketLocation(SpellComp->GetLeftHandCastSocket());

	const FVector TraceEnd = CameraLocation + CameraRotation.Vector() * Aim_Trace_Length;

	FHitResult HitResult;
	bool IsHitSomething = GenHandler.GerOwner()->GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Vehicle, QueryParams);

	Rotation = IsHitSomething ? (HitResult.Location - Location).Rotation() : CameraRotation;
	Rotation =  GMMisc::ClampRotator(GenHandler.GerOwner()->GetActorRotation(), Rotation, Spawn_Rotation_Limit);

	Rotation = FMath::VRandCone(Rotation.Vector(), FMath::DegreesToRadians((Focus_Max - Focus) * Max_Spread)).Rotation();

	return IsHitSomething;
}
