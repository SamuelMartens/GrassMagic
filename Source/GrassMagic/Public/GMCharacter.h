// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GMCharacter.generated.h"

UCLASS()
class GRASSMAGIC_API AGMCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGMCharacter();

protected:

	// Only bindings names that used more than once get here
	const static FName Move_Forward_Axis_Binding_Name;
	const static FName Move_Right_Axis_Binding_Name;
	const static FName Turn_Axis_Binding_Name;
	const static FName LookUp_Axis_Binding_Name;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	virtual void MoveForward(float Value);

	/** Called for side to side input */
	virtual void MoveRight(float Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
