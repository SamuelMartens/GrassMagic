// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrassMagic.h"

/**
 * This class implements generic logic for all Input Handlers in Grassmann Algebra.
 * You can treat it as component, which means it is not mandatory for every Input Handler to have it
 */

class UGMSpellComponent;

class GRASSMAGIC_API FGMInputHandlerGeneric
{
public:

	void Init(APawn* NewOwner, UGMSpellComponent* SpellComp, void (UGMSpellComponent::*CallBackRelease)());

	TWeakObjectPtr<APawn> GerOwner() const { return Owner; }

	void ExecuteReleaseCallBack() 
	{
		check(CallBackGenericInputRelease.IsBound());

		CallBackGenericInputRelease.Execute();
	};

private:
	TWeakObjectPtr<APawn> Owner;
	
	DelVoidVoid CallBackGenericInputRelease;
};
