// Copyright Epic Games, Inc. All Rights Reserved.

#include "PR_ResistanceCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"

#include "PR_Resistance/StatesSystem/StateManager_Player.h"

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

	mStateManager = std::make_shared<StateManager_Player>(2);
	mStateManager->SetProvider(this);


	Rifle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rifle"));
	Rifle->SetupAttachment(GetMesh(),TEXT("SM_Rifle"));

	// add to archive

	

}

APR_ResistanceCharacter::~APR_ResistanceCharacter()
{

}

void APR_ResistanceCharacter::Landed(const FHitResult& Hit)
{	
	Super::Landed(Hit);
	mStateManager->SetStateEnd(CharacterState::CS_JUMP);
}

void APR_ResistanceCharacter::StartWait()
{
	mTimeToNextStepNotifier();
}

//////////////////////////////////////////////////////////////////////////
// Input
void APR_ResistanceCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APR_ResistanceCharacter::Jump_Wrapped);
	PlayerInputComponent->

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
	PlayerInputComponent->BindAction("Run", IE_Repeat, this, &APR_ResistanceCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APR_ResistanceCharacter::RunStop);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APR_ResistanceCharacter::DoJumpDash);

	//Dodge
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &APR_ResistanceCharacter::Dodge);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APR_ResistanceCharacter::StartAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &APR_ResistanceCharacter::StopAttack);


	PlayerInputComponent->BindAction("Weapon1", IE_Pressed, this, &APR_ResistanceCharacter::SetWeapon1);
	PlayerInputComponent->BindAction("Weapon2", IE_Pressed, this, &APR_ResistanceCharacter::SetWeapon2);
}

void APR_ResistanceCharacter::BeginPlay()
{
	ACharacter::BeginPlay();
	
	//
	GetCharacterMovement()->MaxWalkSpeed = mStatus.walkSpeed;
	
	// PlayerStates Init
	mStateManager->AddArchiveData("Status", &mStatus);
	mStateManager->AddArchiveData("MovementSpeed", &GetCharacterMovement()->MaxWalkSpeed);
	mStateManager->AddArchiveData("CharacterVelocity", &GetCharacterMovement()->Velocity);
	mStateManager->AddArchiveData("CharacterGravityScale", &GetCharacterMovement()->GravityScale);
	mStateManager->AddArchiveData("LastInputVector", &mLastInputVector);
	mStateManager->AddArchiveData("MovementComponent", GetMovementComponent());
	mStateManager->AddArchiveData("AnimInstance", GetMesh()->GetAnimInstance());
	mStateManager->AddArchiveData("ActionTable", mActionDataTable);
	mStateManager->AddArchiveData("AInput_Change", &mLastInput);
	mStateManager->AddArchiveData("TimeToNextStepNotify", &mTimeToNextStepNotifier);
	mStateManager->AddArchiveData("World", GetWorld());
	mStateManager->AddArchiveData("CharacterTransform", const_cast<FTransform*>(&GetTransform()));
	mStateManager->AddArchiveData("StaticMeshComponenet", Rifle);
	

	mStateManager->Init();

}

void APR_ResistanceCharacter::Tick(float deltaTime)
{
	mStateManager->TryChangeState(CharacterState::CS_IDLE);

	mStateManager->Update(deltaTime);
	FucDynamicOneParam.Broadcast(mStatus.curStamina / mStatus.maxStamina);

	// 
 	mLastInputVector = GetCharacterMovement()->GetLastInputVector();

	GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Red,FString::Printf(TEXT("CurState : %d"), mStateManager->GetCurStateDesc().StateType));


	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("bIsMeele : %d"), bIsMeele));
	
	// 효과적이고 간단하게 바꾸기
	if (bIsMeele)
	{
		mStateManager->ChangeState(StateType::ST_SWORD);
	}
	else
	{
		mStateManager->ChangeState(StateType::ST_GUN);
	}
}

// Jump
void APR_ResistanceCharacter::Jump_Wrapped()
{
	if (JumpCurrentCount < 2)
	{
		float forUseStamina = mStatus.maxStamina * (JumpCurrentCount == 0 ? 0.07f : 0.08f);
		if (UseStamina(forUseStamina))
		{
			Jump();
			mStateManager->TryChangeState(CharacterState::CS_JUMP);
		}
	}

}

void APR_ResistanceCharacter::Dodge()
{
	mStateManager->TryChangeState(CharacterState::CS_DODGE);
}

void APR_ResistanceCharacter::DoJumpDash()
{
	mStateManager->TryChangeState(CharacterState::CS_RUN);
	mStateManager->TryChangeState(CharacterState::CS_JUMPDASH);
}

// Run
void APR_ResistanceCharacter::Run()
{
	mStateManager->TryChangeState(CharacterState::CS_RUN);
}

void APR_ResistanceCharacter::RunStop()
{
	mStateManager->SetStateEnd(CharacterState::CS_RUN);
}


void APR_ResistanceCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void APR_ResistanceCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
		mStateManager->SetStateEnd(CharacterState::CS_JUMP);
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
		mStateManager->SetStateEnd(CharacterState::CS_WALK);
	}
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
		mStateManager->SetStateEnd(CharacterState::CS_WALK);
	}
}

void APR_ResistanceCharacter::StartAttack()
{
	mLastInput = ActionInput::AINPUT_WEAKATTACK;
	bIsInAttack = true;

	mStateManager->TryChangeState(CharacterState::CS_ATTACK);
}

void APR_ResistanceCharacter::StopAttack()
{
	if (!bIsMeele)
	{
		mStateManager->SetStateEnd(CharacterState::CS_ATTACK);
	}
}


void APR_ResistanceCharacter::SetWeapon1()
{
	mStateManager->SetStateEnd(CharacterState::CS_ATTACK);
}

void APR_ResistanceCharacter::SetWeapon2()
{
	mStateManager->SetStateEnd(CharacterState::CS_ATTACK);
}

///////////Interface
bool APR_ResistanceCharacter::UseStamina(float usedStamina)
{
	if (mStatus.curStamina < usedStamina)
		return false;

	mStatus.curStamina -= usedStamina;
	FucDynamicOneParam.Broadcast(mStatus.curStamina / mStatus.maxStamina);

	return true;
}
