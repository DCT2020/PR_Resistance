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
#include "Kismet/GameplayStatics.h"

#include "PR_Resistance/StatesSystem/Managers/StateManager_Player.h"

//////////////////////////////////////////////////////////////////////////
// APR_ResistanceCharacter

APR_ResistanceCharacter::APR_ResistanceCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
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
	

	// 라이플 StaticMesh
	
	Rifle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Rifle"));
	if (Rifle)
	{
		Rifle->AlwaysLoadOnClient = true;
		Rifle->AlwaysLoadOnServer = true;
		Rifle->bOwnerNoSee = false;
		Rifle->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		Rifle->bCastDynamicShadow = true;
		Rifle->bAffectDynamicIndirectLighting = true;
		Rifle->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Rifle->SetupAttachment(GetMesh(), TEXT("SM_Rifle"));
		Rifle->SetCollisionProfileName(TEXT("Rifle"));
		Rifle->SetGenerateOverlapEvents(false);
		Rifle->SetCanEverAffectNavigation(false);
	}
	// 근접 무기 StaticMesh
	MeleeWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeleeWeapon"));
	MeleeWeapon->SetupAttachment(GetMesh(), TEXT("SM_Melee"));

	mStateManager = CreateDefaultSubobject<UStateManager_Player>(TEXT("FSM"));
	mStateManager->SetProvider(this);

	// add to archive


	
}

APR_ResistanceCharacter::~APR_ResistanceCharacter()
{

}

void APR_ResistanceCharacter::Landed(const FHitResult& Hit)
{	
	Super::Landed(Hit);
	mStateManager->SetStateEnd((uint8)CharacterState::CS_JUMP);
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

	PlayerInputComponent->BindAxis("MoveForward", this, &APR_ResistanceCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APR_ResistanceCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn", this, &APR_ResistanceCharacter::Turn);
	PlayerInputComponent->BindAxis("TurnRate", this, &APR_ResistanceCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APR_ResistanceCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APR_ResistanceCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &APR_ResistanceCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APR_ResistanceCharacter::TouchStopped);

	// Run
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APR_ResistanceCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APR_ResistanceCharacter::RunStop);

	//Dodge
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &APR_ResistanceCharacter::Dodge);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APR_ResistanceCharacter::StartAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &APR_ResistanceCharacter::StopAttack);


	PlayerInputComponent->BindAction("Weapon1", IE_Pressed, this, &APR_ResistanceCharacter::SetWeapon1);
	PlayerInputComponent->BindAction("Weapon2", IE_Pressed, this, &APR_ResistanceCharacter::SetWeapon2);

	////////////////////////////// SubState //////////////////////////////////////////
	//Reload
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APR_ResistanceCharacter::Reload);

	//Aim
	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &APR_ResistanceCharacter::StartAiming);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &APR_ResistanceCharacter::EndAiming);
}

void APR_ResistanceCharacter::BeginPlay()
{
	ACharacter::BeginPlay();
	
	mStateManager->Init();


	//
	GetCharacterMovement()->MaxWalkSpeed = mStatus.walkSpeed;
	
	// PlayerStates Init
	mStateManager->AddArchiveData("Status", &mStatus);
	mStateManager->AddArchiveData("MovementSpeed", &GetCharacterMovement()->MaxWalkSpeed);
	mStateManager->AddArchiveData("CharacterVelocity", &GetCharacterMovement()->Velocity);
	mStateManager->AddArchiveData("CharacterGravityScale", &GetCharacterMovement()->GravityScale);
	mStateManager->AddArchiveData("LastInputVector", &LastControlInputVector);
	mStateManager->AddArchiveData("MovementComponent", GetMovementComponent());
	mStateManager->AddArchiveData("AnimInstance", GetMesh()->GetAnimInstance());
	mStateManager->AddArchiveData("ActionTable", mActionDataTable);
	mStateManager->AddArchiveData("AInput_Change", &mLastInput);
	mStateManager->AddArchiveData("TimeToNextStepNotify", &mTimeToNextStepNotifier);
	mStateManager->AddArchiveData("World", GetWorld());
	mStateManager->AddArchiveData("CharacterTransform", const_cast<FTransform*>(&GetTransform()));
	mStateManager->AddArchiveData("SkeletalMeshComponent", Rifle);
	
	// Load states
	mStateManager->LoadStates();



	// weapon collision (나중에 Rifle에서 MeleeWeapon으로 바꿀 것)	
	Rifle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Rifle->OnComponentBeginOverlap.AddDynamic(this, &APR_ResistanceCharacter::OnWeaponOverlaped);
	Rifle->SetGenerateOverlapEvents(true);
}

void APR_ResistanceCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (mStateManager->GetCurStateDesc().StateType == (uint8)CharacterState::CS_IDLE)
	{
		bIsIdle = true;
	}
	else
	{
		bIsIdle = false;
	}

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
			mStateManager->TryChangeState((uint8)CharacterState::CS_JUMP);
		}
	}

}

void APR_ResistanceCharacter::Dodge()
{
	mStateManager->TryChangeState((uint8)CharacterState::CS_DODGE);
}

void APR_ResistanceCharacter::DoJumpDash()
{
	mStateManager->TryChangeState((uint8)CharacterState::CS_RUN);
	mStateManager->TryChangeState((uint8)CharacterState::CS_JUMPDASH);
}

// Run
void APR_ResistanceCharacter::Run()
{
	mStateManager->SetState((uint8)CharacterState::CS_RUN);
	mStateManager->SetState((uint8)CharacterState::CS_JUMPDASH);
}

void APR_ResistanceCharacter::RunStop()
{
	mStateManager->SetStateEnd((uint8)CharacterState::CS_RUN);
}


void APR_ResistanceCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void APR_ResistanceCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
	mStateManager->SetStateEnd((uint8)CharacterState::CS_JUMP);
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
	if (bIsMeele)
	{
		if (mStateManager->GetCurStateDesc().StateType == (uint8)CharacterState::CS_ATTACK)
			return;
	}
	
	bool bIsMoved = (Controller != NULL) && (Value != 0.0f);
	if (bIsMoved)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		mStateManager->TryChangeState((uint8)CharacterState::CS_WALK);
	}
	else
	{
		mStateManager->SetStateEnd((uint8)CharacterState::CS_WALK);
	}
}

void APR_ResistanceCharacter::MoveRight(float Value)
{
	if (bIsMeele)
	{
		if (mStateManager->GetCurStateDesc().StateType == (uint8)CharacterState::CS_ATTACK)
			return;
	}
	
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

		mStateManager->TryChangeState((uint8)CharacterState::CS_WALK);
	}
	else
	{
		mStateManager->SetStateEnd((uint8)CharacterState::CS_WALK);
	}
}

void APR_ResistanceCharacter::StartAttack()
{
	mLastInput = ActionInput::AINPUT_WEAKATTACK;
	bIsInAttack = true;

	mStateManager->TryChangeState((uint8)CharacterState::CS_ATTACK);
}

void APR_ResistanceCharacter::StopAttack()
{
	if (!bIsMeele)
	{
		mStateManager->SetStateEnd((uint8)CharacterState::CS_ATTACK);
	}
}


void APR_ResistanceCharacter::SetWeapon1()
{
	mStateManager->SetStateEnd((uint8)CharacterState::CS_ATTACK);
}

void APR_ResistanceCharacter::SetWeapon2()
{
	mStateManager->SetStateEnd((uint8)CharacterState::CS_ATTACK);
}

void APR_ResistanceCharacter::Turn(float var)
{
	APawn::AddControllerYawInput(var);
	if (bIsAim)
	{
		FRotator rot = Controller->GetControlRotation();
		rot.Pitch = 0.0f;
		RootComponent->SetWorldRotation(rot);
	}
}

void APR_ResistanceCharacter::LookUp(float var)
{
	APawn::AddControllerPitchInput(var);
}

void APR_ResistanceCharacter::Reload()
{
	mStateManager->TryChangeSubState(CharacterState::CS_SUB_RELOAD);
}

void APR_ResistanceCharacter::StartAiming()
{
	mStateManager->TryChangeSubState(CharacterState::CS_SUB_AIM);
}

void APR_ResistanceCharacter::EndAiming()
{
	mStateManager->SetSubStateEnd(CharacterState::CS_SUB_AIM);
}

void APR_ResistanceCharacter::OnWeaponOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::ApplyDamage(OtherActor,10.0f,GetController(),this,NULL);
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

void APR_ResistanceCharacter::ReceiveNotification(EAnimNotifyToCharacterTypes curNotiType, bool bIsEnd)
{
	switch (curNotiType)
	{
	case EAnimNotifyToCharacterTypes::ATC_ATTACK:
		if (bIsEnd)
		{
			// 현재 라이플이지만 검으로 사용하고 있다.
			Rifle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else
		{
			Rifle->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		break;
	case EAnimNotifyToCharacterTypes::ATC_RELOAD_COMPLITE:
		mStateManager->SetSubStateEnd(CharacterState::CS_SUB_RELOAD);
		break;
	default:
		break;
	}
}
/////////// bps
void APR_ResistanceCharacter::GetCurrentCharacterState_bp(CharacterState& state)
{
	state = (CharacterState)mStateManager->GetCurStateDesc().StateType;
}

void APR_ResistanceCharacter::GetCurrentCharacterSubState_bp(CharacterState& subState)
{
	subState = mStateManager->GetCurSubState();
}
