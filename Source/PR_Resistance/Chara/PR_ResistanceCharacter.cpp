// Copyright Epic Games, Inc. All Rights Reserved.

#include "PR_ResistanceCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "PR_Resistance/StatesSystem/StateManager.h"

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
	//mStateManager = NewObject()
	mStateManager = std::make_shared<StateManager>();
	mStateManager->Init();
}

APR_ResistanceCharacter::~APR_ResistanceCharacter()
{

}

void APR_ResistanceCharacter::BeginPlay()
{
	ACharacter::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(handle,[this](){
	
		GEngine->AddOnScreenDebugMessage(-1,0.1f,FColor::Blue,
			mStateManager->GetCurStateDesc().StateType == CharacterState::CS_IDLE ? TEXT("State : CS_IDLE") : TEXT("State : CS_WALK"));
	},0.1f,true);
	// PlayerStates Init
}

void APR_ResistanceCharacter::Tick(float deltaTime)
{
	//curState->Update(deltaTime);
	mStateManager->Update(deltaTime	);
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
	bool bIsMoved = (Controller != NULL) && (Value != 0.0f);
	if (bIsMoved)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		mStateManager->TryChangeState(CharacterState::CS_WALK);
	}
	else
	{
		mStateManager->TryChangeState(CharacterState::CS_IDLE);
	}


	//CheckIsMove(true, bIsMoved);
}

void APR_ResistanceCharacter::MoveRight(float Value)
{
	bool bIsMoved = (Controller != NULL) && (Value != 0.0f);
	if (bIsMoved)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);


		mStateManager->TryChangeState(CharacterState::CS_WALK);
	}
	else
	{
		mStateManager->TryChangeState(CharacterState::CS_IDLE);
	}
}


// TODO : 개선의 여지를 찾자
// Right와 Forward라는 2가지 움직임 상태가 있는데 만약 Forward입력만 들어오면 right은 움직이지 않은상태여서
// 상태가 idle로 된다. 이를 해결하기위해 Walk State로의 변경을 함수 한곳에서 관리한다.
// State를 walk상태로 하기 위한 함수다.
void APR_ResistanceCharacter::CheckIsMove(bool bIsForward, bool bIsMoved)
{
	static bool forwardTrigger = false;
	static bool rightTrigger = false;

	if (bIsForward)
	{
		if (bIsMoved)
		{
			forwardTrigger = true;
		}
		else
		{
			forwardTrigger = false;
		}
	}
	else
	{
		if (bIsMoved)
		{
			rightTrigger = true;
		}
		else
		{
			rightTrigger = false;
		}
	}

	//mStateManager->
	//curState = (forwardTrigger || rightTrigger) ? States[1] : States[0];
}

//////////////////////////
void APR_ResistanceCharacter::SetSpeed(float speed)
{
	GetCharacterMovement()->MaxWalkSpeed = speed;
}
