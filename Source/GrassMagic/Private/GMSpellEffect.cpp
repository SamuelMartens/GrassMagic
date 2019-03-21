#include "GMSpellEffect.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "AIModule/Classes/AIController.h"

#include "GMHealthComponent.h"
#include "GMSpellEffectManagerComponent.h"
#include "GMWorldSettings.h"
#include "GMSpellComponent.h"
#include "GMMisc.h"


const static float Dummy_Period = 1.0f;
const static FName Color_Param_Name = FName("color");

/*-------------- Damage ------------------- */

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

/*-------------- Change ------------------- */

const float UGMSpellEffect_Change_1::Particle_Activation_Tick_Interval = 1.2f;
const float UGMSpellEffect_Change_1::Particle_Orientation_Tick_Interval = 0.05f;

const FName UGMSpellEffect_Change_1::Attach_Socket = FName("Above");
const float UGMSpellEffect_Change_1::Custom_Time_Dilation = 0.2f;
const FVector UGMSpellEffect_Change_1::Particle_Effect_Scale = FVector(0.1f);
const FRotator UGMSpellEffect_Change_1::Particle_Effect_Init_Rotation = FRotator(-135.0f, 90.0f, -90.0f);

const float UGMSpellEffect_Change_1::Damage_Resist_Multiplier = 0.8f;
const float UGMSpellEffect_Change_1::Time_Length_Multiplier = 10.0f;


const float UGMSpellEffect_Change_2::Movement_Multiplier = 0.5f;
const float UGMSpellEffect_Change_2::Time_Length_Multiplier = 2.0f;

const FName UGMSpellEffect_Change_2::Attach_Socket_Left = FName("calf_lSocket");
const FName UGMSpellEffect_Change_2::Attach_Socket_Right = FName("calf_rSocket");
const FVector UGMSpellEffect_Change_2::Particle_Effect_Scale = FVector(0.1f);
const FRotator UGMSpellEffect_Change_2::Particle_Effect_Rotation = FRotator(90.0f, 0.0f, 0.0f);
const float UGMSpellEffect_Change_2::Custom_Time_Dilation = 0.4f;


const float UGMSpellEffect_Change_3::Damage_Multiplier = 0.5f;
const float UGMSpellEffect_Change_3::Time_Length_Multiplier = 2.0f;

const FName UGMSpellEffect_Change_3::Attach_Socket_Left = FName("hand_lSocket");
const FName UGMSpellEffect_Change_3::Attach_Socket_Right = FName("hand_rSocket");
const FVector UGMSpellEffect_Change_3::Particle_Effect_Scale = FVector(0.1f);
const FRotator UGMSpellEffect_Change_3::Particle_Effect_Rotation = FRotator(90.0f, 0.0f, 0.0f);
const float UGMSpellEffect_Change_3::Custom_Time_Dilation = 0.4f;

/*-------------- Control ------------------- */

const FVector UGMSpellEffect_Control_1::Particle_Effect_Scale = FVector(0.5f);
const FRotator UGMSpellEffect_Control_1::Particle_Effect_Rotation = FRotator(90.0f, 0.0f, 0.0f);
const float UGMSpellEffect_Control_1::Custom_Time_Dilation = 0.7f;

const float UGMSpellEffect_Control_1::Displacement_Multiplier = 150.0f;


const FVector UGMSpellEffect_Control_2::Particle_Effect_Scale = FVector(0.5f);
const float UGMSpellEffect_Control_2::Custom_Time_Dilation = 0.6f;

const float UGMSpellEffect_Control_2::Stun_Length_Multiplier = 1.0f;


const FVector UGMSpellEffect_Control_3::Particle_Effect_Scale = FVector(0.3f);
const float UGMSpellEffect_Control_3::Custom_Time_Dilation = 0.7f;
const float UGMSpellEffect_Control_3::Particle_Effect_Activation_Interval = 0.8f;
const FName UGMSpellEffect_Control_3::Attach_Socket = FName("headSocket");

const float UGMSpellEffect_Control_3::Control_Length_Multiplier = 1.0f;

namespace
{
	// All these functions exist only to avoid code duplication

	void DestroyComponentGeneric(UActorComponent* Comp)
	{
		Comp->UnregisterComponent();
		Comp->DestroyComponent();
	}

	template<typename T>
	T* DuplicateComponent(const APawn* Source, APawn* Target, USceneComponent* AttachComp = nullptr, FName AttachSocket = NAME_None)
	{
		static_assert(std::is_base_of<UActorComponent, T>::value, "Type should be derived from ActorComponent");
		// Just make sure that component that we are trying to create are not already exists for target actor.
		// In the situation where I use this function it is actually pretty useful, but if it will
		// be a problem then it's ok to move it outside of this function to place where it is actually required.
		// As far as I know UE4 is totally fine with actor having a few components of the same type
		check(Target->GetComponentByClass(T::StaticClass()) == nullptr);

		T* CompSource = Cast<T>(Source->GetComponentByClass(T::StaticClass()));
		check(CompSource);

		T* CompTarget = DuplicateObject<T>(CompSource, Target);
		check(CompTarget);

		if (AttachComp)
			CompTarget->SetupAttachment(AttachComp, AttachSocket);
		else
			Target->SetRootComponent(AttachComp);

		CompTarget->OnComponentCreated();
		CompTarget->RegisterComponent();

		return CompTarget;
	}
}


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

	UGMSpellEffectManagerComponent* EffectManager = GMMisc::GetCompByClassCheck<UGMSpellEffectManagerComponent>(Holder);

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

	USkeletalMeshComponent* SkeletalComp = GMMisc::GetCompByClassCheck<USkeletalMeshComponent>(Holder);

	ParticleEffect = UGameplayStatics::SpawnEmitterAttached(WorldSetting->DamageGrade1, SkeletalComp, Attach_Socket);

	ParticleEffect->SetVectorParameter(Color_Param_Name, UGMSpellComponent::Damage_Gesture_Effect_Color);
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

	USkeletalMeshComponent* SkeletalComp = GMMisc::GetCompByClassCheck<USkeletalMeshComponent>(Holder);

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
	
	DestroyComponentGeneric(ParticleEffect);

	Super::Die();
}



void UGMSpellEffect_Damage_3::Start()
{
	check(Holder);

	AGMWorldSettings* WorldSetting = Cast<AGMWorldSettings>(Holder->GetWorld()->GetWorldSettings());
	check(WorldSetting && WorldSetting->DamageGrade3);

	USkeletalMeshComponent* SkeletalComp = GMMisc::GetCompByClassCheck<USkeletalMeshComponent>(Holder);

	ParticleEffect = UGameplayStatics::SpawnEmitterAttached(WorldSetting->DamageGrade3, SkeletalComp, 
		NAME_None, FVector(ForceInit), FRotator::ZeroRotator, Particle_Effect_Scale);
	ParticleEffect->CustomTimeDilation = Custom_Time_Dilation_1;
	ParticleEffect->SetVectorParameter(Color_Param_Name, UGMSpellComponent::Damage_Gesture_Effect_Color);

	ParticleEffect = UGameplayStatics::SpawnEmitterAttached(WorldSetting->DamageGrade3, SkeletalComp,
		NAME_None, FVector(ForceInit), Particle_Effect_Rotation_2, Particle_Effect_Scale);
	ParticleEffect->CustomTimeDilation = Custom_Time_Dilation_2;
	ParticleEffect->SetVectorParameter(Color_Param_Name, Particle_Effect_Color_2);

	UGameplayStatics::ApplyDamage(Holder, Value, Instigator->GetController(), Instigator, DummyDamageType);

	Die();
}


void UGMSpellEffect_Change_1::Start()
{
	check(Holder);

	AGMWorldSettings* WorldSetting = Cast<AGMWorldSettings>(Holder->GetWorld()->GetWorldSettings());
	check(WorldSetting && WorldSetting->ChangeGrade1);

	USkeletalMeshComponent* SkeletalComp = GMMisc::GetCompByClassCheck<USkeletalMeshComponent>(Holder);

	ParticleEffect = UGameplayStatics::SpawnEmitterAttached(WorldSetting->ChangeGrade1, SkeletalComp, Attach_Socket);
	ParticleEffect->bAutoDestroy = false;
	ParticleEffect->SetVectorParameter(Color_Param_Name, UGMSpellComponent::Change_Gesture_Effect_Color);
	ParticleEffect->CustomTimeDilation = Custom_Time_Dilation;
	ParticleEffect->RelativeScale3D = Particle_Effect_Scale;
	ParticleEffect->bAbsoluteRotation = true;
	ParticleEffect->RelativeRotation = Particle_Effect_Init_Rotation;

	UGMHealthComponent* HealthComp = GMMisc::GetCompByClassCheck<UGMHealthComponent>(Holder);

	HealthComp->SetDamageResist(HealthComp->GetDamageResist() * Damage_Resist_Multiplier);
	
	Holder->GetWorldTimerManager().SetTimer(TimerHandler_OnDie, this, &UGMSpellEffect_Change_1::Die, Dummy_Period,
		false, Value * Time_Length_Multiplier);
	
	Holder->GetWorldTimerManager().SetTimer(TimerHandler_ParticleActivation, this,
		&UGMSpellEffect_Change_1::OnTimerTick_ParticleActivation, Particle_Activation_Tick_Interval, Particle_Activation_Tick_Interval);

	Holder->GetWorldTimerManager().SetTimer(TimerHanlder_ParticleOrientation, this,
		&UGMSpellEffect_Change_1::OnTimerTick_ParticleOrientation, Particle_Orientation_Tick_Interval, true, 0.0f);
}

/*-------------- Change ------------------- */


void UGMSpellEffect_Change_1::OnTimerTick_ParticleActivation()
{
	ParticleEffect->Activate(true);
}

void UGMSpellEffect_Change_1::OnTimerTick_ParticleOrientation()
{
	const FRotator Rot = (Holder->GetActorLocation() - Instigator->GetActorLocation()).Rotation();
	ParticleEffect->RelativeRotation.Yaw = Particle_Effect_Init_Rotation.Yaw + Rot.Yaw;
}

void UGMSpellEffect_Change_1::Die()
{
	Holder->GetWorldTimerManager().ClearTimer(TimerHandler_ParticleActivation);
	Holder->GetWorldTimerManager().ClearTimer(TimerHanlder_ParticleOrientation);
	Holder->GetWorldTimerManager().ClearTimer(TimerHandler_OnDie);

	DestroyComponentGeneric(ParticleEffect);

	UGMHealthComponent* HealthComp = GMMisc::GetCompByClassCheck<UGMHealthComponent>(Holder);
	HealthComp->SetDamageResist(HealthComp->GetDamageResist() / Damage_Resist_Multiplier);

	Super::Die();
}


void UGMSpellEffect_Change_2::Start()
{
	check(Holder);

	AGMWorldSettings* WorldSetting = Cast<AGMWorldSettings>(Holder->GetWorld()->GetWorldSettings());
	check(WorldSetting && WorldSetting->ChangeGrade2);

	USkeletalMeshComponent* SkeletalComp = GMMisc::GetCompByClassCheck<USkeletalMeshComponent>(Holder);

	UCharacterMovementComponent* CharMoveComp = GMMisc::GetCompByClassCheck<UCharacterMovementComponent>(Holder);

	ParticleEffectLeft = UGameplayStatics::SpawnEmitterAttached(WorldSetting->ChangeGrade2, SkeletalComp, Attach_Socket_Left);
	ParticleEffectLeft->RelativeScale3D = Particle_Effect_Scale;
	ParticleEffectLeft->RelativeRotation = Particle_Effect_Rotation;
	ParticleEffectLeft->SetVectorParameter(Color_Param_Name, UGMSpellComponent::Change_Gesture_Effect_Color);
	ParticleEffectLeft->CustomTimeDilation = Custom_Time_Dilation;

	ParticleEffectRight = UGameplayStatics::SpawnEmitterAttached(WorldSetting->ChangeGrade2, SkeletalComp, Attach_Socket_Right);
	ParticleEffectRight->RelativeScale3D = Particle_Effect_Scale;
	ParticleEffectRight->RelativeRotation = Particle_Effect_Rotation;
	ParticleEffectRight->SetVectorParameter(Color_Param_Name, UGMSpellComponent::Change_Gesture_Effect_Color);
	ParticleEffectRight->CustomTimeDilation = Custom_Time_Dilation;

	CharMoveComp->MaxWalkSpeed *= Movement_Multiplier;

	Holder->GetWorldTimerManager().SetTimer(TimerHandler_OnDie, this, &UGMSpellEffect_Change_2::Die, Dummy_Period,
		false, Value * Time_Length_Multiplier);
}

void UGMSpellEffect_Change_2::Die()
{
	ParticleEffectLeft->Deactivate();
	ParticleEffectRight->Deactivate();

	Holder->GetWorldTimerManager().ClearTimer(TimerHandler_OnDie);

	UCharacterMovementComponent* CharMoveComp = GMMisc::GetCompByClassCheck<UCharacterMovementComponent>(Holder);

	CharMoveComp->MaxWalkSpeed /= Movement_Multiplier;

	Super::Die();
}


void UGMSpellEffect_Change_3::Start()
{
	check(Holder);

	AGMWorldSettings* WorldSetting = Cast<AGMWorldSettings>(Holder->GetWorld()->GetWorldSettings());
	check(WorldSetting && WorldSetting->ChangeGrade3);

	USkeletalMeshComponent* SkeletalComp = GMMisc::GetCompByClassCheck<USkeletalMeshComponent>(Holder);

	ParticleEffectLeft = UGameplayStatics::SpawnEmitterAttached(WorldSetting->ChangeGrade3, SkeletalComp, Attach_Socket_Left);
	ParticleEffectLeft->RelativeScale3D = Particle_Effect_Scale;
	ParticleEffectLeft->RelativeRotation = Particle_Effect_Rotation;
	ParticleEffectLeft->SetVectorParameter(Color_Param_Name, UGMSpellComponent::Change_Gesture_Effect_Color);
	ParticleEffectLeft->CustomTimeDilation = Custom_Time_Dilation;

	ParticleEffectRight = UGameplayStatics::SpawnEmitterAttached(WorldSetting->ChangeGrade3, SkeletalComp, Attach_Socket_Right);
	ParticleEffectRight->RelativeScale3D = Particle_Effect_Scale;
	ParticleEffectRight->RelativeRotation = Particle_Effect_Rotation;
	ParticleEffectRight->SetVectorParameter(Color_Param_Name, UGMSpellComponent::Change_Gesture_Effect_Color);
	ParticleEffectRight->CustomTimeDilation = Custom_Time_Dilation;

	//#TODO: implement damage reducing logic here
	
	Holder->GetWorldTimerManager().SetTimer(TimerHandler_OnDie, this, &UGMSpellEffect_Change_3::Die, Dummy_Period,
		false, Value * Time_Length_Multiplier);
}

void UGMSpellEffect_Change_3::Die()
{
	ParticleEffectLeft->Deactivate();
	ParticleEffectRight->Deactivate();

	//#TODO implement damage restoring logic here

	Super::Die();
}

/*-------------- Control ------------------- */


void UGMSpellEffect_Control_1::Start()
{
	check(Holder);

	AGMWorldSettings* WorldSetting = Cast<AGMWorldSettings>(Holder->GetWorld()->GetWorldSettings());
	check(WorldSetting && WorldSetting->ControlGrade1);

	FTransform SpawnTransform;
	SpawnTransform.SetScale3D(Particle_Effect_Scale);
	SpawnTransform.SetRotation((ProjectileImpactRotation + Particle_Effect_Rotation).Quaternion());
	SpawnTransform.SetLocation(ProjectileImpactPosition);

	ParticleEffect = UGameplayStatics::SpawnEmitterAtLocation(Holder->GetWorld(), WorldSetting->ControlGrade1, SpawnTransform);
	ParticleEffect->CustomTimeDilation = Custom_Time_Dilation;

	Cast<ACharacter>(Holder)->LaunchCharacter(ProjectileImpactRotation.Vector() * (Value * Displacement_Multiplier), false, false);

	Die();
}


void UGMSpellEffect_Control_2::Start()
{
	check(Holder);

	AGMWorldSettings* WorldSetting = Cast<AGMWorldSettings>(Holder->GetWorld()->GetWorldSettings());
	check(WorldSetting && WorldSetting->ControlGrade2);

	USkeletalMeshComponent* SkeletalComp = GMMisc::GetCompByClassCheck<USkeletalMeshComponent>(Holder);

	ParticleEffect = UGameplayStatics::SpawnEmitterAttached(WorldSetting->ControlGrade2, SkeletalComp, NAME_None);
	ParticleEffect->RelativeScale3D = Particle_Effect_Scale;
	ParticleEffect->CustomTimeDilation = Custom_Time_Dilation;

	HolderController = Cast<AAIController>(Holder->GetController());

	if (HolderController)
		HolderController->UnPossess();

	Holder->GetWorldTimerManager().SetTimer(TimerHandler_OnDie, this, &UGMSpellEffect_Control_2::Die, Dummy_Period, false,
		Value * Stun_Length_Multiplier);
}

void UGMSpellEffect_Control_2::Die()
{
	ParticleEffect->Deactivate();

	if (HolderController && !TryPassControllerResponsibility())
		HolderController->Possess(Holder);

	Holder->GetWorldTimerManager().ClearTimer(TimerHandler_OnDie);

	Super::Die();

}

bool UGMSpellEffect_Control_2::TryPassControllerResponsibility()
{
	check(Holder);

	// If don't own holder controller we can't pass it
	if (!HolderController)
		return false;

	UGMSpellEffectManagerComponent* EffectManager = GMMisc::GetCompByClassCheck<UGMSpellEffectManagerComponent>(Holder);

	UGMSpellEffect_Control_2* OtherEffectOfSameType = EffectManager->GetEffectOfType<UGMSpellEffect_Control_2>(GetId());

	if (OtherEffectOfSameType)
	{
		OtherEffectOfSameType->SetHolderController(HolderController);
		return true;
	}

	return false;
}


void UGMSpellEffect_Control_3::Start()
{
	check(Holder);

	AGMWorldSettings* WorldSetting = Cast<AGMWorldSettings>(Holder->GetWorld()->GetWorldSettings());
	check(WorldSetting && WorldSetting->ControlGrade3);

	// Set up particles
	USkeletalMeshComponent* SkeletalCompHolder = GMMisc::GetCompByClassCheck<USkeletalMeshComponent>(Holder);
	USkeletalMeshComponent* SkeletalCompInst = GMMisc::GetCompByClassCheck<USkeletalMeshComponent>(Instigator);

	ParticleEffectHolder = UGameplayStatics::SpawnEmitterAttached(WorldSetting->ControlGrade3, SkeletalCompHolder, 
		Attach_Socket, FVector(ForceInit), FRotator::ZeroRotator, Particle_Effect_Scale);

	ParticleEffectHolder->CustomTimeDilation = Custom_Time_Dilation;
	ParticleEffectHolder->bAbsoluteRotation = true;
	ParticleEffectHolder->bAutoDestroy = false;

	ParticleEffectInstigator = UGameplayStatics::SpawnEmitterAttached(WorldSetting->ControlGrade3, SkeletalCompInst,
		Attach_Socket, FVector(ForceInit), FRotator::ZeroRotator, Particle_Effect_Scale);

	ParticleEffectInstigator->CustomTimeDilation = Custom_Time_Dilation;
	ParticleEffectInstigator->bAbsoluteRotation = true;
	ParticleEffectInstigator->bAutoDestroy = false;

	// Create required components
	CreateComponentsToControl();

	// Set up proper controller possession
	HolderController = Holder->GetController();
	InstigatorController = Instigator->GetController();

	HolderController->UnPossess();
	InstigatorController->UnPossess();

	InstigatorController->Possess(Holder);

	// Set up timers
	Holder->GetWorldTimerManager().SetTimer(TimerHandler_OnDie, this, &UGMSpellEffect_Control_3::Die, Dummy_Period, false,
		Value * Control_Length_Multiplier);

	Holder->GetWorldTimerManager().SetTimer(TimerHandler_ParticleActivation, this, &UGMSpellEffect_Control_3::OnParticleEffectActivate,
		Particle_Effect_Activation_Interval, true, Particle_Effect_Activation_Interval);

	// Set up additional properties (required to make movement smooth)
	UMovementComponent* MovementCompInst = GMMisc::GetCompByClassCheck<UMovementComponent>(Instigator);

}

void UGMSpellEffect_Control_3::Die()
{
	Holder->GetWorldTimerManager().ClearTimer(TimerHandler_OnDie);
	Holder->GetWorldTimerManager().ClearTimer(TimerHandler_ParticleActivation);

	InstigatorController->UnPossess();

	InstigatorController->Possess(Instigator);
	HolderController->Possess(Holder);

	ParticleEffectHolder->Deactivate();
	DestroyComponentGeneric(ParticleEffectHolder);

	ParticleEffectInstigator->Deactivate();
	DestroyComponentGeneric(ParticleEffectInstigator);
	
	UCameraComponent* CameraCompHolder = GMMisc::GetCompByClassCheck<UCameraComponent>(Holder);
	DestroyComponentGeneric(CameraCompHolder);

	USpringArmComponent* SpringArmCompHolder = GMMisc::GetCompByClassCheck<USpringArmComponent>(Holder);
	DestroyComponentGeneric(SpringArmCompHolder);	
	
	Super::Die();
}

void UGMSpellEffect_Control_3::OnParticleEffectActivate()
{
	ParticleEffectHolder->Activate(true);
	ParticleEffectInstigator->Activate(true);
}

void UGMSpellEffect_Control_3::CreateComponentsToControl()
{	
	USpringArmComponent* SpringArmCompHolder = DuplicateComponent<USpringArmComponent>(Instigator, Holder, Holder->GetRootComponent());

	DuplicateComponent<UCameraComponent>(Instigator, Holder, SpringArmCompHolder, USpringArmComponent::SocketName);
}
