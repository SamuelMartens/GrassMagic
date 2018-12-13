// Fill out your copyright notice in the Description page of Project Settings.

#include "GMSpellCaster.h"
#include "GMGestures.h"



void UGMSpellCaster::Init(AActor* OwnerActor)
{
	check(OwnerActor);
	Owner = OwnerActor;
}

void UGMSpellCaster::StartDamageGesture()
{
	CurrentCastGesture = FGMDamageGesture::Inst().GetAllTypes();
	// Here is specialized function for time grow goes, we start it by timer
}

void UGMSpellCaster::StopDamageGesture()
{
	// We stop timer and mix new shader with base
	CurrentCastGesture = FVector(0, 0, 0);
}

void UGMSpellCaster::OnTickGestureCast()
{

}
