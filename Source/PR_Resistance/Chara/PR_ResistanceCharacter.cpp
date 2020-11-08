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
#include "Net/UnrealNetwork.h"

#include "PR_Resistance/StatesSystem/Managers/StateManager_Player.h"

//////////////////////////////////////////////////////////////////////////
// APR_ResistanceCharacter

void APR_ResistanceCharacter::PlaySlotAnimation_onServrer_Implementation(FName slotName,
	UAnimSequenceBase* anim, int index) {
	PlaySlotAnimation(slotName, anim,index);
}

void APR_ResistanceCharacter::StopSlotAnimation_onServrer_Implementation(FName slotName) {
	StopSlotAnimation(slotName);
}

void APR_ResistanceCharacter::PlaySlotAnimation_Implementation(FName slotName, UAnimSequenceBase* anim, int index)
{
	UKismetSystemLibrary::PrintString(this, TEXT("Multicast"));
	  if(!HasAuthority()) {
		  GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("MT"));

	  }

	  mActiveMontage = GetMesh()->GetAnimInstance()->PlaySlotAnimationAsDynamicMontage(anim, slotName,0.1f,0.1f);
}

void APR_ResistanceCharacter::StopSlotAnimation_Implementation(FName slotName)
{
        if(mActiveMontage != nullptr) {
			GetMesh()->GetAnimInstance()->Montage_Play(mActiveMontage);
        }
	GetMesh()->GetAnimInstance()->StopSlotAnimation(0.25f, slotName);
}


void APR_ResistanceCharacter::Montage_PauseOnServer_Implementation(int index) {
	Montage_PauseMulticast(index);
}

void APR_ResistanceCharacter::Montage_PauseMulticast_Implementation(int index) {
        GetMesh()->GetAnimInstance()->Montage_Pause(mActiveMontage);
}

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

	// float 관리 컴포넌트
	mFloatsComponent = CreateDefaultSubobject<UFloatsComponent>(TEXT("FloatManager"));

	// 데미지 처리 콜백 함수 등록
	OnTakeAnyDamage.AddDynamic(this, &APR_ResistanceCharacter::OntTakeDamage);

	// 라이플 StaticMesh
	
	mRifle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Rifle_Skeletal"));
	if (mRifle)
	{
		mRifle->AlwaysLoadOnClient = true;
		mRifle->AlwaysLoadOnServer = true;
		mRifle->bOwnerNoSee = false;
		mRifle->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		mRifle->bCastDynamicShadow = true;
		mRifle->bAffectDynamicIndirectLighting = true;
		mRifle->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		mRifle->SetupAttachment(GetMesh(), TEXT("SM_Rifle"));
		mRifle->SetCollisionProfileName(TEXT("Rifle"));
		mRifle->SetGenerateOverlapEvents(false);
		mRifle->SetCanEverAffectNavigation(false);
	}
	// 근접 무기 StaticMesh
	MeleeWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeleeWeapon"));
	MeleeWeapon->SetupAttachment(GetMesh(), TEXT("SM_Melee"));

	mStateManager = CreateDefaultSubobject<UStateManager_Player>(TEXT("FSM"));
	mStateManager->SetProvider(this);
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

	PlayerInputComponent->BindAxis(TEXT("MoveForward_"), this, &APR_ResistanceCharacter::MoveForward);
	
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

	PlayerInputComponent->BindAction("StrongAttack", IE_Pressed, this, &APR_ResistanceCharacter::StrongAttack);

}

void APR_ResistanceCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APR_ResistanceCharacter, mPrevForwardInput);
	DOREPLIFETIME(APR_ResistanceCharacter, mPrevRightInput);
	DOREPLIFETIME(APR_ResistanceCharacter, bIsMeele); 
	DOREPLIFETIME(APR_ResistanceCharacter, bIsCanAttack); 
}

void APR_ResistanceCharacter::BeginPlay()
{
	ACharacter::BeginPlay();
	
	mStateManager->Init();
		RootComponent->SetIsReplicated(true);


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
	mStateManager->AddArchiveData("SkeletalMeshComponent", mRifle);
	mStateManager->AddArchiveData("RootComponent", RootComponent);
	mStateManager->AddArchiveData("AnimTable", mAnimTable);
	mStateManager->AddArchiveData("Owner", const_cast<APR_ResistanceCharacter*>(this));
	mStateManager->AddArchiveData("SoundTable", mSoundTable);
	mStateManager->AddArchiveData("FireEffect", mFireEffect);
	mStateManager->AddArchiveData("RifleMesh", mRifle);
	mStateManager->AddArchiveData("Floats", mFloatsComponent);
	mStateManager->AddArchiveData("Camera", FollowCamera);
	// Load states
	mStateManager->LoadStates();

	GetCharacterMovement()->MaxWalkSpeed = mStatus.walkSpeed;

	// weapon collision (나중에 Rifle에서 MeleeWeapon으로 바꿀 것)	
	mRifle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mRifle->OnComponentBeginOverlap.AddDynamic(this, &APR_ResistanceCharacter::OnWeaponOverlaped);
	mRifle->SetGenerateOverlapEvents(true);

	// floats 등록
	mFloatsComponent->PushBack(mStatus.curHP);
	mFloatsComponent->AddListener(this, 0);
	mFloatsComponent->PushBack(mStatus.CurAmmo);
	mFloatsComponent->AddListener(this, 1);
	
	// hitmotion
	mHitMotion = mAnimTable->FindRow<FCharacterAnimationData>(TEXT("Hit"), nullptr)->mAnimation;
	// spawn rifle motion
	mSpawnRifleMotion = mAnimTable->FindRow <FCharacterAnimationData > (TEXT("SpawnRifle"), nullptr)->mAnimation;
	mDeSpawnRifleMotion = mAnimTable->FindRow <FCharacterAnimationData >(TEXT("DeSpawnRifle"), nullptr)->mAnimation;
	
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &APR_ResistanceCharacter::OnHitAnimEnd);

	//replicate
	RootComponent->SetIsReplicated(true);
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

	//GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::Red,FString::Printf(TEXT("CurState : %d"), mStateManager->GetCurStateDesc().StateType));
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("bIsMeele : %d"), bIsMeele));
	
	// 효과적이고 간단하게 바꾸기
	if (bIsMeele)
	{
		mStateManager->ChangeState(StateType::ST_SWORD);
	}
	else
	{
		mStateManager->ChangeState(StateType::ST_GUN);
	}

	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%s] IsMeele? : %d"),*this->GetName(),bIsMeele),true,true,FLinearColor::Blue,0.0f);
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%s] SubState : %d"), *this->GetName(), mStateManager->GetCurSubState()), true, true, FLinearColor::Blue, 0.0f);
}

#pragma  region INPUT_BINDED_FUNCTIONS

// Jump
void APR_ResistanceCharacter::Jump_Wrapped_Implementation()
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

void APR_ResistanceCharacter::Dodge_Implementation()
{
	StopSlotAnimation_onServrer(TEXT("UpperMotion"));
	StopSlotAnimation_onServrer(TEXT("ParallelMotion"));
	//GetMesh()->GetAnimInstance()->StopSlotAnimation(0.0f, );
	//GetMesh()->GetAnimInstance()->StopSlotAnimation(0.0f, TEXT("ParallelMotion"));
	mStateManager->TryChangeState((uint8)CharacterState::CS_DODGE);
}

void APR_ResistanceCharacter::DoJumpDash()
{
	mStateManager->TryChangeState((uint8)CharacterState::CS_RUN);
	mStateManager->TryChangeState((uint8)CharacterState::CS_JUMPDASH);
}

// Run
void APR_ResistanceCharacter::Run_Implementation()
{
	mStateManager->SetState((uint8)CharacterState::CS_RUN);
	mStateManager->SetState((uint8)CharacterState::CS_JUMPDASH);
}

void APR_ResistanceCharacter::RunStop_Implementation()
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
	mPrevForwardInput = Value;

	if (mStateManager->GetCurStateDesc().StateType == (uint8)CharacterState::CS_HIT)
		mPrevForwardInput = 0.0f;;
	
	if (mStateManager->GetCurStateDesc().StateType == (uint8)CharacterState::CS_DODGE)
		mPrevForwardInput = 0.0f;;
	
	if (bIsMeele)
	{
		if (mStateManager->GetCurStateDesc().StateType == (uint8)CharacterState::CS_ATTACK)
			mPrevForwardInput = 0.0f;;
	}

	UpdatePrevForwardInput(mPrevForwardInput);

	bool bIsMoved = (Controller != NULL) && (Value != 0.0f);
	if (bIsMoved)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, FString::Printf(TEXT("%f"), GetMovementComponent()->GetMaxSpeed()));
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

void APR_ResistanceCharacter::UpdatePrevForwardInput_Implementation(float value)
{
	mPrevForwardInput = value;
}

void APR_ResistanceCharacter::MoveRight(float Value)
{
	mPrevRightInput = Value;

	if (mStateManager->GetCurStateDesc().StateType == (uint8)CharacterState::CS_HIT)
		mPrevRightInput = 0;

	if (mStateManager->GetCurStateDesc().StateType == (uint8)CharacterState::CS_DODGE)
		mPrevRightInput = 0;

	if (bIsMeele)
	{
		if (mStateManager->GetCurStateDesc().StateType == (uint8)CharacterState::CS_ATTACK)
			mPrevRightInput = 0;
	}
	
	UpdatePrevRightInput(mPrevRightInput);

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

void APR_ResistanceCharacter::UpdatePrevRightInput_Implementation(float value)
{
	mPrevRightInput = value;
}

void APR_ResistanceCharacter::StartAttack_Implementation()
{
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%s] Attack Call"), *this->GetName()));


	if (!bIsCanAttack)
		return;

	mLastInput = ActionInput::AINPUT_WEAKATTACK;
	bIsInAttack = true;

	mStateManager->TryChangeState((uint8)CharacterState::CS_ATTACK);
}

void APR_ResistanceCharacter::StopAttack_Implementation()
{
	if (!bIsMeele)
	{
		mStateManager->SetStateEnd((uint8)CharacterState::CS_ATTACK);
	}
}

void APR_ResistanceCharacter::StrongAttack_Implementation()
{
	if (!bIsCanAttack)
		return;
	if (bIsMeele)
	{
		mLastInput = ActionInput::AINPUT_STRONGATTACK;
		bIsInAttack = true;
		mStateManager->TryChangeState((uint8)CharacterState::CS_ATTACK);
	}
}

void APR_ResistanceCharacter::SetWeapon1_Implementation()
{
	if (mStateManager->GetCurStateDesc().StateType == (uint8)CharacterState::CS_DODGE)
		return;

	PlaySlotAnimation_onServrer(TEXT("UpperMotion"), mDeSpawnRifleMotion);
	//GetMesh()->GetAnimInstance()->PlaySlotAnimationAsDynamicMontage(mDeSpawnRifleMotion, TEXT("UpperMotion"));
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[%s] SetWeapon1 Called? : %d"), *this->GetName(), bIsMeele));

	bIsCanAttack = false;
	bIsMeele = true;
}

void APR_ResistanceCharacter::SetWeapon2_Implementation()
{
	if (mStateManager->GetCurStateDesc().StateType == (uint8)CharacterState::CS_DODGE)
		return;

	PlaySlotAnimation_onServrer(TEXT("UpperMotion"), mSpawnRifleMotion);
	//GetMesh()->GetAnimInstance()->PlaySlotAnimationAsDynamicMontage(mSpawnRifleMotion, TEXT("UpperMotion"));
	bIsCanAttack = false;
	bIsMeele = false;
}

void APR_ResistanceCharacter::Turn_Implementation(float var)
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

void APR_ResistanceCharacter::Reload_Implementation()
{
	mStateManager->TryChangeSubState(CharacterState::CS_SUB_RELOAD);
}

void APR_ResistanceCharacter::StartAiming_Implementation()
{
	if (!bIsMeele)
		mStateManager->TryChangeSubState(CharacterState::CS_SUB_AIM);
}

void APR_ResistanceCharacter::EndAiming_Implementation()
{
	if (!bIsMeele)
		mStateManager->SetSubStateEnd(CharacterState::CS_SUB_AIM);
}

#pragma  endregion 

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

void APR_ResistanceCharacter::ListenFloat(int index, float newFloat)
{
    if(index == 0) 
    {
      if(mStatus.curHP > newFloat)
	{
		// 공격 받았다.
		mStateManager->TryChangeState((uint8)CharacterState::CS_HIT);

		bIsParallelMotionValid = true;
	}

	mStatus.curHP = newFloat;
	if(mStatus.curHP < 0.0f)
	{
		mStatus.curHP = 0.0f;
	}  
    }
    else if(index ==1) 
    {
	mStatus.CurAmmo = newFloat;
    }
}

void APR_ResistanceCharacter::OnHitAnimEnd(UAnimMontage* motange, bool bInterrupted)
{
	bIsParallelMotionValid = false;
	bIsCanAttack = true;
}

void APR_ResistanceCharacter::ReceiveNotification(EAnimNotifyToCharacterTypes curNotiType, bool bIsEnd)
{
	switch (curNotiType)
	{
	case EAnimNotifyToCharacterTypes::ATC_ATTACK:
		if (bIsEnd)
		{
			mRifle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else
		{
			mRifle->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		break;
	case EAnimNotifyToCharacterTypes::ATC_RELOAD_COMPLITE:
		if(bIsEnd)
			mStateManager->SetSubStateEnd(CharacterState::CS_SUB_RELOAD);
		break;
	default:
		break;
	}
}

/*
 * On Damage
 * 
 */
void APR_ResistanceCharacter::OntTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (mStateManager->GetCurStateDesc().StateType == (uint8)CharacterState::CS_DODGE)
		return;

	float hp = 0.0f;
	mFloatsComponent->Get(0, hp);
	mFloatsComponent->Set(hp - Damage,0);

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("hitted"));
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
