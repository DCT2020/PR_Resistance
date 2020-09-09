// Copyright Epic Games, Inc. All Rights Reserved.

#include "PR_ResistanceCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "PR_Resistance/StatesSystem/IState.h"
#include "PR_Resistance/StatesSystem/Idle.h"
#include "PR_Resistance/StatesSystem/Walk.h"

//////////////////////////////////////////////////////////////////////////
// APR_ResistanceCharacter

APR_ResistanceCharacter::APR_ResistanceCharacter()
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
	if (States[0] == nullptr)
		States[0] = new Idle(&mStatus);
	if (States[1] == nullptr)
		States[1] = new Walk(&mStatus);

	curState = States[1];
}

APR_ResistanceCharacter::~APR_ResistanceCharacter()
{
	for (int i = 0; i < 2; ++i)
	{
		if(States[i] != nullptr)
			delete States[i];
	}
}

void APR_ResistanceCharacter::BeginPlay()
{
	ACharacter::BeginPlay();
	
	// PlayerStates Init
}

void APR_ResistanceCharacter::Tick(float deltaTime)
{
	curState->Update(deltaTime);
}

//////////////////////////////////////////////////////////////////////////
// Input

void APR_ResistanceCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APR_ResistanceCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APR_ResistanceCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APR_ResistanceCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APR_ResistanceCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &APR_ResistanceCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APR_ResistanceCharacter::TouchStopped);

	// Run
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APR_ResistanceCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APR_ResistanceCharacter::RunStop);


}

// Run
void APR_ResistanceCharacter::Run()
{
	SetSpeed(mStatus.runSpeed);
}

void APR_ResistanceCharacter::RunStop()
{
	SetSpeed(mStatus.walkSpeed);
}
//

void APR_ResistanceCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void APR_ResistanceCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void APR_ResistanceCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APR_ResistanceCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APR_ResistanceCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		curState = States[1];
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	else
	{
		curState = States[0];
	}
}

void APR_ResistanceCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		curState = States[1];
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

	}
	else
	{
		curState = States[0];
	}
}


//////////////////////////
void APR_ResistanceCharacter::SetSpeed(float speed)
{
	GetCharacterMovement()->MaxWalkSpeed = speed;
}
