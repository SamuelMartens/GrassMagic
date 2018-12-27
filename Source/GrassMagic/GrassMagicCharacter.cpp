// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GrassMagicCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "GMSpellComponent.h"


const float AGrassMagicCharacter::Input_Value_Per_Tick = 1.0f;

//////////////////////////////////////////////////////////////////////////
// AGrassMagicCharacter

AGrassMagicCharacter::AGrassMagicCharacter()
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
}

void AGrassMagicCharacter::BeginPlay()
{

	Super::BeginPlay();

	SpellComponent->Init(Input_Value_Per_Tick);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGrassMagicCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Resources
	PlayerInputComponent->BindAction("AcquireResources", IE_Pressed, this, &AGrassMagicCharacter::AcquireResources<IE_Pressed>);
	PlayerInputComponent->BindAction("AcquireResources", IE_Released, this, &AGrassMagicCharacter::AcquireResources<IE_Released>);

	// Gestures
	PlayerInputComponent->BindAction("DamageGesture", IE_Pressed, this, &AGrassMagicCharacter::HandleDamageGesture<IE_Pressed>);
	PlayerInputComponent->BindAction("DamageGesture", IE_Released, this, &AGrassMagicCharacter::HandleDamageGesture<IE_Released>);

	PlayerInputComponent->BindAction("ControlGesture", IE_Pressed, this, &AGrassMagicCharacter::HandleControlGesture<IE_Pressed>);
	PlayerInputComponent->BindAction("ControlGesture", IE_Released, this, &AGrassMagicCharacter::HandleControlGesture<IE_Released>);

	PlayerInputComponent->BindAction("ChangeGesture", IE_Pressed, this, &AGrassMagicCharacter::HandleControlGesture<IE_Pressed>);
	PlayerInputComponent->BindAction("ChangeGesture", IE_Released, this, &AGrassMagicCharacter::HandleControlGesture<IE_Released>);


	PlayerInputComponent->BindAxis("MoveForward", this, &AGrassMagicCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGrassMagicCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AGrassMagicCharacter::MoveForward(float Value)
{
	Value = SpellComponent->AdjustMovement(Value);

	if (Controller == NULL || Value == 0.0f)
		return;

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AGrassMagicCharacter::MoveRight(float Value)
{
	Value = SpellComponent->AdjustMovement(Value);

	if (Controller == NULL || Value == 0.0f)
		return;

	// find out which way is right
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// add movement in that direction
	
	AddMovementInput(Direction, Value);
}

void AGrassMagicCharacter::HandleDamageGesture(EInputEvent InputEvent)
{
	SpellComponent->HandleDamageGesture(InputEvent);
}

void AGrassMagicCharacter::HandleControlGesture(EInputEvent InputEvent)
{

}

void AGrassMagicCharacter::HandleChangeGesture(EInputEvent InputEvent)
{

}

void AGrassMagicCharacter::AcquireResource(EInputEvent inputEvent)
{
	SpellComponent->HandleAcquireResource(inputEvent);
}
