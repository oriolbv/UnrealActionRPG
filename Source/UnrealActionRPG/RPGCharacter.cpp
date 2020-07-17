// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacter.h"

// Sets default values
ARPGCharacter::ARPGCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;

	// Don't rotate when the controller rotates. //Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	// Character moves in the direction of input...
	GetCharacterMovement()->bOrientRotationToMovement = true; // ...at this rotation rate  
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	// The camera follows at this distance behind the character  CameraBoom->TargetArmLength = 300.0f; 
	CameraBoom->RelativeLocation = FVector(0.f, 0.f, 500.f);// Rotate the arm based on the controller
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Camera does not rotate relative to arm
	FollowCamera->bUsePawnControlRotation = false;  FollowCamera->RelativeRotation = FRotator(-45.f, 0.f, 0.f);

	/* Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)*/


}



//////////////////////////////////////////////////////////////////
// Input

void ARPGCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ARPGCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARPGCharacter::MoveRight);

	/* We have 2 versions of the rotation bindings to handle different kinds of devices differently "turn" handles devices that provide an absolute delta, such as a mouse. "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick*/
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ARPGCharacter::TurnAtRate);
}


void ARPGCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPGCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARPGCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

