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
	StopGestureGeneric(FGMDamageGesture::Inst().GetBase(), FGMDamageGesture::Inst().GetDominantType());
}

void UGMSpellCaster::StartGestureGeneric()
{
	CastStartTime = FTimespan::FromSeconds(UGameplayStatics::GetRealTimeSeconds(Owner->GetWorld()));
}

void UGMSpellCaster::StopGestureGeneric(const FVector& GestureVector, FGMBaseGesture::EType Type)
{
	FTimespan CastTime = FTimespan::FromSeconds(UGameplayStatics::GetRealTimeSeconds(Owner->GetWorld())) - CastStartTime;
	const FVector ScaledGestureVector = GestureVector * CastTime.GetTotalMilliseconds() * 0.005f;

	State.AddEffect(ScaledGestureVector, Type);
}

