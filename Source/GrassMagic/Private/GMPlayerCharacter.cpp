// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GMPlayerCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "GMSpellComponent.h"
#include "GMHealthComponent.h"


const float AGMPlayerCharacter::Input_Value_Per_Tick = 1.0f;

//////////////////////////////////////////////////////////////////////////
// AGrassMagicCharacter

AGMPlayerCharacter::AGMPlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	SpellComponent = CreateDefaultSubobject<UGMSpellComponent>(TEXT("SpellComponent"));
	HealthComponent = CreateDefaultSubobject<UGMHealthComponent>(TEXT("HealthComponent"));
}

void AGMPlayerCharacter::BeginPlay()
{

	Super::BeginPlay();

	SpellComponent->Init(Input_Value_Per_Tick);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGMPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Resources
	PlayerInputComponent->BindAction("AcquireResources", IE_Pressed, SpellComponent, &UGMSpellComponent::HandleAcquireResource_Pressed);
	PlayerInputComponent->BindAction("AcquireResources", IE_Released, SpellComponent, &UGMSpellComponent::HandleAcquireResource_Released);

	// Gestures
	PlayerInputComponent->BindAction("DamageGesture", IE_Pressed, SpellComponent, &UGMSpellComponent::HandleDamageGesture_Pressed);
	PlayerInputComponent->BindAction("DamageGesture", IE_Released, SpellComponent, &UGMSpellComponent::HandleDamageGesture_Released);

	PlayerInputComponent->BindAction("ControlGesture", IE_Pressed, SpellComponent, &UGMSpellComponent::HandleControlGesture_Pressed);
	PlayerInputComponent->BindAction("ControlGesture", IE_Released, SpellComponent, &UGMSpellComponent::HandleControlGesture_Released);

	PlayerInputComponent->BindAction("ChangeGesture", IE_Pressed, SpellComponent, &UGMSpellComponent::HandleChangeGesture_Pressed);
	PlayerInputComponent->BindAction("ChangeGesture", IE_Released, SpellComponent, &UGMSpellComponent::HandleChangeGesture_Released);

	// Spell Release
	PlayerInputComponent->BindAction("ReleaseSpell", IE_Pressed, SpellComponent, &UGMSpellComponent::HandleReleaseSpell_Pressed);
	PlayerInputComponent->BindAction("ReleaseSpell", IE_Released, SpellComponent, &UGMSpellComponent::HandleReleaseSpell_Released);

	PlayerInputComponent->BindAxis(Move_Forward_Axis_Binding_Name, this, &AGMPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(Move_Right_Axis_Binding_Name, this, &AGMPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis(Turn_Axis_Binding_Name, this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(LookUp_Axis_Binding_Name, this, &APawn::AddControllerPitchInput);
}

void AGMPlayerCharacter::MoveForward(float Value)
{
	Value = SpellComponent->AdjustMovement(Value);

	Super::MoveForward(Value);
}

void AGMPlayerCharacter::MoveRight(float Value)
{
	Value = SpellComponent->AdjustMovement(Value);

	Super::MoveRight(Value);
}