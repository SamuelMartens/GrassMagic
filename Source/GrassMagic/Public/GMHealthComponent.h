// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GMHealthComponent.generated.h"
// Just a reminder for myself:
// Delegate should be dynamic cause then it could be serialized and so used in blueprint,
// Delegate should be Multi-cast so we can attach a few funtions to it
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, Health);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDeathSignature);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRASSMAGIC_API UGMHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGMHealthComponent();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthDeathSignature OnDeath;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const noexcept { return Health; }

	float GetDamageResist() const { return DamageResist; }

	void SetDamageResist(float val) { DamageResist = val; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Resist", 
		meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float DamageResist = 1.0f;
	
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
private:
	
	float Health;
};
