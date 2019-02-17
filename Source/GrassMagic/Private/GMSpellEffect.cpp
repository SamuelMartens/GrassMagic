#include "GMSpellEffect.h"

#include "Kismet/GameplayStatics.h"

#include "GMHealthComponent.h"
#include "GMSpellEffectManagerComponent.h"

const float UGMSpellEffect_Damage_1::Tick_Interval = 0.1f;

/*-------------- Base class ------------------- */

UGMBaseSpellEffect::UGMBaseSpellEffect() :
	Id(GenerateId())
{};

UGMBaseSpellEffect::UGMBaseSpellEffect(const FObjectInitializer& Initializer) :
	Super(Initializer),
	Id(GenerateId())
{};


void UGMBaseSpellEffect::Init(APawn* EffectHolder, APawn* EffectInstigator, float EffectValue)
{
	check(EffectHolder);
	check(EffectInstigator);

	Holder = EffectHolder;
	Instigator = EffectInstigator;

	Value = EffectValue;
}

void UGMBaseSpellEffect::Die()
{
	check(Holder);

	UGMSpellEffectManagerComponent* EffectManager = Cast<UGMSpellEffectManagerComponent>
		(Holder->GetComponentByClass(UGMSpellEffectManagerComponent::StaticClass()));
	check(EffectManager);

	EffectManager->RemoveEffect(GetId());
}


/*-------------- Damage ------------------- */


void UGMSpellEffect_Damage_1::Die()
{
	Holder->GetWorldTimerManager().ClearTimer(TimerHandler_DamageTick);

	Super::Die();
}


void UGMSpellEffect_Damage_1::Start()
{
	check(Holder);

	Holder->GetWorldTimerManager().SetTimer(TimerHandler_DamageTick, this,
		&UGMSpellEffect_Damage_1::OnDamageTick, Tick_Interval, true);
}

void UGMSpellEffect_Damage_1::OnDamageTick()
{

	UGameplayStatics::ApplyDamage(Holder, Value, Instigator->GetController(), Instigator, DummyDamageType);

	if (++TicksDone >= Ticks_Num)
		Die();
}