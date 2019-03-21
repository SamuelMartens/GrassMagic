#pragma once

#include "CoreMinimal.h"

#include "GMGestures.h"
#include "GMSpellComponent.h"

#include "GMSpellEffect.generated.h"

class UParticleSystemComponent;
class AAIController;
class AController;

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
	//#DEBUG
	float GetValue() { return Value; }
	//END

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

/*-------------- Change ------------------- */

UCLASS()
class UGMSpellEffect_Change_1 : public UGMBaseSpellEffect
{
	GENERATED_BODY()

	const static float Particle_Activation_Tick_Interval;
	const static float Particle_Orientation_Tick_Interval;

	const static FName Attach_Socket;
	const static float Custom_Time_Dilation;
	const static FVector Particle_Effect_Scale;
	const static FRotator Particle_Effect_Init_Rotation;

	const static float Damage_Resist_Multiplier;
	const static float Time_Length_Multiplier;
	
public:

	virtual void Start() override;

	UFUNCTION()
	void OnTimerTick_ParticleActivation();

	UFUNCTION()
	void OnTimerTick_ParticleOrientation();

protected:
	UFUNCTION()
	virtual void Die() override;

private:

	UParticleSystemComponent* ParticleEffect = nullptr;

	FTimerHandle TimerHandler_ParticleActivation;
	FTimerHandle TimerHanlder_ParticleOrientation;
	FTimerHandle TimerHandler_OnDie;

};

UCLASS()
class UGMSpellEffect_Change_2 : public UGMBaseSpellEffect
{
	GENERATED_BODY()

	const static FName Attach_Socket_Left;
	const static FName Attach_Socket_Right;
	const static FVector Particle_Effect_Scale;
	const static FRotator Particle_Effect_Rotation;
	const static float Custom_Time_Dilation;

	const static float Movement_Multiplier;
	const static float Time_Length_Multiplier;

public:

	virtual void Start() override;

protected:

	UFUNCTION()
	virtual void Die() override;

private:
	
	UParticleSystemComponent* ParticleEffectLeft = nullptr;
	UParticleSystemComponent* ParticleEffectRight = nullptr;

	FTimerHandle TimerHandler_OnDie;
};

UCLASS()
class UGMSpellEffect_Change_3 : public UGMBaseSpellEffect
{
	GENERATED_BODY()

	const static FName Attach_Socket_Left;
	const static FName Attach_Socket_Right;
	const static FVector Particle_Effect_Scale;
	const static FRotator Particle_Effect_Rotation;
	const static float Custom_Time_Dilation;

	const static float Damage_Multiplier;
	const static float Time_Length_Multiplier;

public:

	virtual void Start() override;

protected:

	UFUNCTION()
	virtual void Die() override;

private:

	UParticleSystemComponent* ParticleEffectLeft = nullptr;
	UParticleSystemComponent* ParticleEffectRight = nullptr;

	FTimerHandle TimerHandler_OnDie;

};

/*-------------- Control ------------------- */

UCLASS()
class UGMSpellEffect_Control_1 : public UGMBaseSpellEffect
{
	GENERATED_BODY()

	const static FVector Particle_Effect_Scale;
	const static FRotator Particle_Effect_Rotation;
	const static float Custom_Time_Dilation;

	const static float Displacement_Multiplier;

public:

	virtual void Start() override;

	void SetProjectileImpactPosition(FVector Value) noexcept
	{
		ProjectileImpactPosition = Value;
	}

	void SetProjectileImpactRotation(FRotator Value) noexcept
	{
		ProjectileImpactRotation = Value;
	}

private:

	UParticleSystemComponent* ParticleEffect;

	// Represents place where projectile that produced this effect has been
	// when overlapped with holder
	FVector ProjectileImpactPosition;
	// Represents rotation that projectile that produced this effect had
	// when overlapped with holder
	FRotator ProjectileImpactRotation;
};


UCLASS()
class UGMSpellEffect_Control_2 : public UGMBaseSpellEffect
{
	GENERATED_BODY()

	const static FVector Particle_Effect_Scale;
	const static float Custom_Time_Dilation;

	const static float Stun_Length_Multiplier;

public:

	virtual void Start() override;
	
	void SetHolderController(AAIController* Value) 
	{
		HolderController = Value;
	};

protected:

	UFUNCTION()
	virtual void Die() override;

private:

	FTimerHandle TimerHandler_OnDie;

	// If target has other stun effect we will pass
	// responsibility for making controller possess holder
	// again to them, so it is possible to chain stun abilities
	bool TryPassControllerResponsibility();

	AAIController* HolderController = nullptr;

	UParticleSystemComponent* ParticleEffect = nullptr;


};


UCLASS()
class UGMSpellEffect_Control_3 : public UGMBaseSpellEffect
{
	GENERATED_BODY()

	const static FVector Particle_Effect_Scale;
	const static float Custom_Time_Dilation;
	const static float Particle_Effect_Activation_Interval;
	const static FName Attach_Socket;

	const static float Control_Length_Multiplier;

public:

	virtual void Start() override;

protected:

	UFUNCTION()
	virtual void Die() override;

	UFUNCTION()
	virtual void OnParticleEffectActivate();

private:

	void CreateComponentsToControl();

	FTimerHandle TimerHandler_OnDie;
	FTimerHandle TimerHandler_ParticleActivation;

	UParticleSystemComponent* ParticleEffectHolder = nullptr;
	UParticleSystemComponent* ParticleEffectInstigator = nullptr;

	AController* HolderController = nullptr;
	AController* InstigatorController = nullptr;

	FRotator HolderRotationRate;
};