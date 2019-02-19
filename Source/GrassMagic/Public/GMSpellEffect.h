#pragma once

#include "CoreMinimal.h"

#include "GMGestures.h"
#include "GMSpellComponent.h"

#include "GMSpellEffect.generated.h"

class UParticleSystemComponent;

/*-------------- Base class ------------------- */

UCLASS()
class UGMBaseSpellEffect : public UObject
{
	GENERATED_BODY()

public:


	UGMBaseSpellEffect();
	UGMBaseSpellEffect(const FObjectInitializer& Initializer);

	void Init(APawn* EffectHolder, APawn* EffectInstigator, float EffectValue);

	virtual void Start() { check(0 && "This class is abstract."); }

	uint64_t GetId() const noexcept { return Id;}

protected:

	APawn* Holder = nullptr;
	APawn* Instigator = nullptr;

	float Value = 0.0f;

	virtual void Die();

	// I don't think I need damage type now, but I need to pass something
	// to apply damage. So this is just dummy damage type
	TSubclassOf<UDamageType> DummyDamageType;

private:

	static uint64_t GenerateId() noexcept
	{
		// This the most simple and hence robust way that I know to handle it
		static uint64_t	 currentId = 0;
		return ++currentId;
	}

	const uint64_t Id = 0;
};

/*-------------- Damage ------------------- */

// Slow dot
UCLASS()
class UGMSpellEffect_Damage_1 :public UGMBaseSpellEffect
{
	GENERATED_BODY()

	const static int Ticks_Num = 30;
	const static float Tick_Interval;

	const static FVector Particle_Effect_Scale;
	const static FName Attach_Socket;
	const static float Custom_Time_Dilation;

public:

	virtual void Start() override;

	UFUNCTION()
	void OnDamageTick();

protected:

	virtual void Die() override;

private:

	UParticleSystemComponent* ParticleEffect = nullptr;

	int TicksDone = 0;

	FTimerHandle TimerHandler_DamageTick;

};

// Square dot
UCLASS()
class UGMSpellEffect_Damage_2 : public UGMBaseSpellEffect
{
	GENERATED_BODY()

	const static int Ticks_Num = 15;
	const static float Tick_Interval_Initial;
	const static float Tick_Interval_Modifier;

	const static FName Attach_Socket;
	const static FVector Particle_Effect_Scale;

public:

	virtual void Start() override;

	UFUNCTION()
	void OnDamageTick();

protected:

	virtual void Die() override;

private:

	UParticleSystemComponent* ParticleEffect = nullptr;

	int TicksDone = 0;

	float LastTickInterval = 0.0f;

	FTimerHandle TimerHandler_DamageTick;
};

// Instant damage
UCLASS()
class UGMSpellEffect_Damage_3 : public UGMBaseSpellEffect
{
	GENERATED_BODY()

	const static FVector Particle_Effect_Scale;

	const static float Custom_Time_Dilation_1;
	const static float Custom_Time_Dilation_2;

	const static FRotator Particle_Effect_Rotation_2;
	const static FVector Particle_Effect_Color_2;

public:

	virtual void Start() override;

private:

	UParticleSystemComponent* ParticleEffect = nullptr;

};