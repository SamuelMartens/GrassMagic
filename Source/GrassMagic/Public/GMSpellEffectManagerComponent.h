// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GMSpellEffect.h"

#include "GMSpellEffectManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRASSMAGIC_API UGMSpellEffectManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGMSpellEffectManagerComponent();

	void AddAndStartEffect(UGMBaseSpellEffect* SpellEffect) 
	{
		Effects.Add(SpellEffect);
		SpellEffect->Start();
	};

	void RemoveEffect(uint64_t Id)
	{
		for (size_t i = 0; i < Effects.Num(); ++i)
		{
			if (Effects[i]->GetId() == Id)
			{
				Effects.RemoveAtSwap(i);
				return;
			}
		}
	}

	template<typename T>
	T* GetEffectOfType(uint64_t IgnoredId)
	{
		for (size_t i = 0; i < Effects.Num(); ++i)
		{
			if (Effects[i]->IsA(T::StaticClass()) && Effects[i]->GetId() != IgnoredId)
				return Cast<T>(Effects[i]);
		}

		return nullptr;
	}



private:

	UPROPERTY()
	TArray<UGMBaseSpellEffect*> Effects;

};
