// Fill out your copyright notice in the Description page of Project Settings.

#include "GMSpellCaster.h"
#include "GMGestures.h"
#include "Kismet/GameplayStatics.h"


void UGMSpellCaster::Init(AActor* OwnerActor)
{
	check(OwnerActor);
	Owner = OwnerActor;
}

void UGMSpellCaster::StartDamageGesture()
{
	StartGestureGeneric();
}

void UGMSpellCaster::StopDamageGesture()
{
	StopGestureGeneric(FGMDamageGesture::Inst().GetBase());
}

void UGMSpellCaster::StartGestureGeneric()
{
	CastStartTime = FTimespan::FromSeconds(UGameplayStatics::GetRealTimeSeconds(Owner->GetWorld()));
}

void UGMSpellCaster::StopGestureGeneric(FVector GestureVector)
{
	FTimespan CastTime = FTimespan::FromSeconds(UGameplayStatics::GetRealTimeSeconds(Owner->GetWorld())) - CastStartTime;
	GestureVector *= CastTime.GetTotalMilliseconds() * 0.005f;
	// And now we are mix dude
}

