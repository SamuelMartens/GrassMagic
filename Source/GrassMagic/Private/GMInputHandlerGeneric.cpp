// Fill out your copyright notice in the Description page of Project Settings.

#include "GMInputHandlerGeneric.h"

#include "GMSpellComponent.h"

void FGMInputHandlerGeneric::Init(APawn* NewOwner, UGMSpellComponent* SpellComp, void (UGMSpellComponent::*CallBackRelease)())
{
	check(NewOwner && SpellComp);

	Owner = NewOwner;

	CallBackGenericInputRelease.BindUObject(SpellComp, CallBackRelease);
}
