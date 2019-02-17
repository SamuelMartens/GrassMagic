// Fill out your copyright notice in the Description page of Project Settings.

#include "GMHealthComponent.h"


// Sets default values for this component's properties
UGMHealthComponent::UGMHealthComponent()
{
	Health = MaxHealth;
}


// Called when the game starts
void UGMHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;

	AActor* Owner = GetOwner();
	check(Owner);

	if (Owner)
		Owner->OnTakeAnyDamage.AddDynamic(this, &UGMHealthComponent::HandleTakeAnyDamage);
}

void UGMHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	Health -= Damage;
	OnHealthChanged.Broadcast(Health);

	if (Health <= 0)
		OnDeath.Broadcast();
}
