// Fill out your copyright notice in the Description page of Project Settings.

#include "GMCharacter.h"


const FName AGMCharacter::Move_Forward_Axis_Binding_Name = FName("MoveForward");
const FName AGMCharacter::Move_Right_Axis_Binding_Name = FName("MoveRight");
const FName AGMCharacter::Turn_Axis_Binding_Name = FName("Turn");
const FName AGMCharacter::LookUp_Axis_Binding_Name = FName("LookUp");


// Sets default values
AGMCharacter::AGMCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGMCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGMCharacter::MoveForward(float Value)
{
	if (Controller == NULL || Value == 0.0f)
		return;

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AGMCharacter::MoveRight(float Value)
{
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

// Called every frame
void AGMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis(Move_Forward_Axis_Binding_Name, this, &AGMCharacter::MoveForward);
	PlayerInputComponent->BindAxis(Move_Right_Axis_Binding_Name, this, &AGMCharacter::MoveRight);

	PlayerInputComponent->BindAxis(Turn_Axis_Binding_Name, this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(LookUp_Axis_Binding_Name, this, &APawn::AddControllerPitchInput);
}

