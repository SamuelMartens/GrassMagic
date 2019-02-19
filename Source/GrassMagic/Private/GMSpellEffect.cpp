#include "GMSpellEffect.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "GMHealthComponent.h"
#include "GMSpellEffectManagerComponent.h"
#include "GMWorldSettings.h"
#include "GMSpellComponent.h"

const float UGMSpellEffect_Damage_1::Tick_Interval = 0.8f;

const FName UGMSpellEffect_Damage_1::Attach_Socket = FName("Chest");
const FVector UGMSpellEffect_Damage_1::Particle_Effect_Scale = FVector(0.65f);
const float UGMSpellEffect_Damage_1::Custom_Time_Dilation = 0.5f;


const float UGMSpellEffect_Damage_2::Tick_Interval_Initial = 2.0f;
const float UGMSpellEffect_Damage_2::Tick_Interval_Modifier = 0.8f;

const FName UGMSpellEffect_Damage_2::Attach_Socket = FName("Chest");
const FVector UGMSpellEffect_Damage_2::Particle_Effect_Scale = FVector(1.0f);


const FVector UGMSpellEffect_Damage_3::Particle_Effect_Scale = FVector(0.3f);

const float UGMSpellEffect_Damage_3::Custom_Time_Dilation_1 = 2.0f;
const float UGMSpellEffect_Damage_3::Custom_Time_Dilation_2 = 1.5f;

const FRotator UGMSpellEffect_Damage_3::Particle_Effect_Rotation_2 = FRotator(0.0, 45.0, 0.0);
const FVector UGMSpellEffect_Damage_3::Particle_Effect_Color_2 = FVector(0.00099, 0.238542, 0.07784) * 255;



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
	ParticleEffect->Deactivate();

	Super::Die();
}


void UGMSpellEffect_Damage_1::Start()
{
	check(Holder);

	AGMWorldSettings* WorldSetting = Cast<AGMWorldSettings>(Holder->GetWorld()->GetWorldSettings());
	check(WorldSetting && WorldSetting->DamageGrade1);

	USkeletalMeshComponent* SkeletalComp = Cast<USkeletalMeshComponent>(Holder->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	check(SkeletalComp);

	ParticleEffect = UGameplayStatics::SpawnEmitterAttached(WorldSetting->DamageGrade1, SkeletalComp, Attach_Socket);

	ParticleEffect->SetVectorParameter(FName("color"), UGMSpellComponent::Damage_Gesture_Effect_Color);
	ParticleEffect->RelativeScale3D = Particle_Effect_Scale;
	ParticleEffect->CustomTimeDilation = Custom_Time_Dilation;

	Holder->GetWorldTimerManager().SetTimer(TimerHandler_DamageTick, this,
		&UGMSpellEffect_Damage_1::OnDamageTick, Tick_Interval, true);
}

void UGMSpellEffect_Damage_1::OnDamageTick()
{
	UGameplayStatics::ApplyDamage(Holder, Value, Instigator->GetController(), Instigator, DummyDamageType);

	if (++TicksDone >= Ticks_Num)
		Die();
}



void UGMSpellEffect_Damage_2::Start()
{
	check(Holder);

	AGMWorldSettings* WorldSetting = Cast<AGMWorldSettings>(Holder->GetWorld()->GetWorldSettings());
	check(WorldSetting && WorldSetting->DamageGrade2);

	USkeletalMeshComponent* SkeletalComp = Cast<USkeletalMeshComponent>(Holder->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	check(SkeletalComp);

	ParticleEffect = UGameplayStatics::SpawnEmitterAttached(WorldSetting->DamageGrade2, SkeletalComp, Attach_Socket);
	ParticleEffect->RelativeScale3D = Particle_Effect_Scale;
	ParticleEffect->bAutoDestroy = false;
	ParticleEffect->Deactivate();

	LastTickInterval = Tick_Interval_Initial;
	
	Holder->GetWorldTimerManager().SetTimer(TimerHandler_DamageTick, this,
		&UGMSpellEffect_Damage_2::OnDamageTick, LastTickInterval, false, 0.0f);
}

void UGMSpellEffect_Damage_2::OnDamageTick()
{
	ParticleEffect->Activate(true);
	UGameplayStatics::ApplyDamage(Holder, Value, Instigator->GetController(), Instigator, DummyDamageType);


	if (++TicksDone >= Ticks_Num)
	{
		Die();
	}
	else
	{
		LastTickInterval *= Tick_Interval_Modifier;

		Holder->GetWorldTimerManager().SetTimer(TimerHandler_DamageTick, this,
			&UGMSpellEffect_Damage_2::OnDamageTick, LastTickInterval, false, LastTickInterval);
	}
}

void UGMSpellEffect_Damage_2::Die()
{
	Holder->GetWorldTimerManager().ClearTimer(TimerHandler_DamageTick);
	ParticleEffect->DestroyComponent();

	Super::Die();
}



void UGMSpellEffect_Damage_3::Start()
{
	check(Holder);

	AGMWorldSettings* WorldSetting = Cast<AGMWorldSettings>(Holder->GetWorld()->GetWorldSettings());
	check(WorldSetting && WorldSetting->DamageGrade3);

	USkeletalMeshComponent* SkeletalComp = Cast<USkeletalMeshComponent>(Holder->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	check(SkeletalComp);

	ParticleEffect = UGameplayStatics::SpawnEmitterAttached(WorldSetting->DamageGrade3, SkeletalComp, 
		NAME_None, FVector(ForceInit), FRotator::ZeroRotator, Particle_Effect_Scale);
	ParticleEffect->CustomTimeDilation = Custom_Time_Dilation_1;
	ParticleEffect->SetVectorParameter(FName("color"), UGMSpellComponent::Damage_Gesture_Effect_Color);

	ParticleEffect = UGameplayStatics::SpawnEmitterAttached(WorldSetting->DamageGrade3, SkeletalComp,
		NAME_None, FVector(ForceInit), Particle_Effect_Rotation_2, Particle_Effect_Scale);
	ParticleEffect->CustomTimeDilation = Custom_Time_Dilation_2;
	ParticleEffect->SetVectorParameter(FName("color"), Particle_Effect_Color_2);

	UGameplayStatics::ApplyDamage(Holder, Value, Instigator->GetController(), Instigator, DummyDamageType);

	Die();
}
