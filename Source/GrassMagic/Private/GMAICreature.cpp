// Fill out your copyright notice in the Description page of Project Settings.

#include "GMAICreature.h"

#include "GMHealthComponent.h"
#include "GMSpellEffectManagerComponent.h"

// Sets default values
AGMAICreature::AGMAICreature()
{
	HealthComp = CreateDefaultSubobject<UGMHealthComponent>(TEXT("HealthComp"));

	SpellEffectManagComp = CreateDefaultSubobject<UGMSpellEffectManagerComponent>(TEXT("Spell Effect Management"));
}

// Called when the game starts or when spawned
void AGMAICreature::BeginPlay()
{
	Super::BeginPlay();
}


