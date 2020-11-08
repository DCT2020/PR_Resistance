// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "PR_Resistance/StatesSystem/Status.h"

//struct
#include "PR_Resistance/Combo/Action.h"
#include "PR_Resistance/StatesSystem/StateDesc.h"

//Interface
#include "PR_Resistance/Interface/IStaminaProvider.h"
#include "PR_Resistance/Noti/Notify/ITimeToNextStepNotify.h"
#include "PR_Resistance/Noti/StateNotify/ReceiverInterface/NotifyState_ToCharacterReceiver.h"

//Components
#include "Components/StaticMeshComponent.h"
#include "PR_Resistance/StatesSystem/Managers/StateManager_Player.h"
#include "PR_Resistance/Component/FloatsComponent.h"

//
#include <functional>
//
#include "PR_Resistance/SlotMotionProcess.h"

#include "PR_ResistanceCharacter.generated.h"

USTRUCT(BlueprintType,Atomic)
struct PR_RESISTANCE_API FCharacterAnimationData : public FTableRowBase
{
	GENERATED_BODY()
		
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Animation, meta = (DisplayName = "Animation"))
		UAnimSequence* mAnimation;
};

USTRUCT(BlueprintType, Atomic)
struct PR_RESISTANCE_API FSoundData : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Animation,meta =(DisplayName = "Sound"))
		USoundBase* mSound;
};


//Dynamic Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_OneParam, float, percent);

UCLASS(config = Game)
class APR_ResistanceCharacter : public ACharacter, public IStaminaProvider, public IITimeToNextStepNotify, public INotifyState_ToCharacterReceiver, public IFloatListener
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* mRifle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeleeWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true", DisplayName = "FloatManager"))
	UFloatsComponent* mFloatsComponent;
	
	virtual void ListenFloat(int index, float newFloat) override;
	
	
private:
	// 내부 변수들
	// combo
	ActionInput mLastInput = ActionInput::AINPUT_NULL;

	//state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	UStateManager_Player* mStateManager;

	UPROPERTY(EditAnywhere, Category = Character, meta = (AllowPrivateAccess = "true", DisplayName = "Animation Table"))
	UDataTable* mAnimTable;

	UPROPERTY(EditAnywhere, Category = Character, meta = (AllowPrivateAccess = "true", DisplayName = "Sound Table"))
	UDataTable* mSoundTable;
	
	UPROPERTY(EditAnywhere, Category = Character, meta = (AllowPrivateAccess = "true", DisplayName = "Fire Effect"))
		UParticleSystem* mFireEffect;
	
	std::function<void()> mTimeToNextStepNotifier;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
		UAnimSequence* testSequence;

	UPROPERTY()
		UAnimMontage* mActiveMontage;

	UFUNCTION(Reliable, Server)
		void PlaySlotAnimation_onServrer(FName slotName, UAnimSequenceBase* anim, int index = 0);
	UFUNCTION(Reliable, Server)
		void StopSlotAnimation_onServrer(FName slotName);
	UFUNCTION(Reliable, Server)
		void Montage_PauseOnServer(int index = 0);

	UFUNCTION(Reliable, NetMulticast)
		void PlaySlotAnimation(FName slotName, UAnimSequenceBase* anim, int index = 0);

       UFUNCTION(Reliable, NetMulticast)
		void StopSlotAnimation(FName slotName);
	UFUNCTION(Reliable, NetMulticast)
		void Montage_PauseMulticast(int index = 0);
public:
	APR_ResistanceCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual ~APR_ResistanceCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsInAttack = false;

	//States
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	FStatus mStatus;

	UPROPERTY(EditAnywhere, Category = Character)
	UDataTable* mActionDataTable;

	UPROPERTY(Replicated,BlueprintReadWrite, Category = Character)
	bool bIsMeele = true;

	UPROPERTY(BlueprintReadWrite, Category = Character)
	bool bIsIdle = true;

	UPROPERTY(BlueprintReadWrite, Category = Character)
	bool bIsAim = false;

	// 스테미나 사용 이벤트 디스패쳐(델리게이트)
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
		FDele_Dynamic_OneParam FucDynamicOneParam;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Character)
		bool bIsParallelMotionValid = false;

	UPROPERTY(Replicated,BlueprintReadOnly, VisibleAnywhere, Category = Input, meta = (DisplayName = "Prev forward input"))
		float mPrevForwardInput = 0.0f;
	UPROPERTY(Replicated,BlueprintReadOnly, VisibleAnywhere, Category = Input, meta = (DisplayName = "Prev right input"))
		float mPrevRightInput = 0.0f;

	UPROPERTY()
		UAnimSequence* mHitMotion = nullptr;
	UPROPERTY()
		UAnimSequence* mSpawnRifleMotion = nullptr;
	UPROPERTY()
		UAnimSequence* mDeSpawnRifleMotion = nullptr;
	
	UFUNCTION()
		void OnHitAnimEnd(UAnimMontage* motange, bool bInterrupted);

        UPROPERTY(Replicated)
	bool bIsCanAttack = true;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);
	UFUNCTION(Reliable,Server)
		void UpdatePrevForwardInput(float value);

	/** Called for side to side input */
	void MoveRight(float Value);
	UFUNCTION(Reliable,Server)
		void UpdatePrevRightInput(float value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);


	/*
	* Run Action
	*/
	UFUNCTION(Reliable, Server)
		void Run();

	UFUNCTION(Reliable, Server)
		void RunStop();

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// Jump
	UFUNCTION(Reliable, Server)
		void Jump_Wrapped();

	// Dodge
	UFUNCTION(Reliable, Server)
		void Dodge();

	// Jump Dash
	void DoJumpDash();

	//Attack
	UFUNCTION(Reliable, Server)
	        void StartAttack();
	UFUNCTION(Reliable, Server)
		void StopAttack();

	//Attack
	UFUNCTION(Reliable, Server)
		void StrongAttack();

	// Swap
	UFUNCTION(Reliable, Server) 
		void SetWeapon1();

	UFUNCTION(Reliable, Server)
		void SetWeapon2();

	UFUNCTION(Reliable,Server);
		void Turn(float var);

	void LookUp(float var);

	//Substate
	// reload
	UFUNCTION(Reliable, Server)
		void Reload();
	// aim
	UFUNCTION(Reliable, Server)
	        void StartAiming();
	UFUNCTION(Reliable, Server)
		void EndAiming();
	//Aim

	// Weapon OverlapBegin
	UFUNCTION()
	virtual void OnWeaponOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetCurrentCharacterState"))
	void GetCurrentCharacterState_bp(CharacterState& state);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetcurrentCharacterSubState"))
	void GetCurrentCharacterSubState_bp(CharacterState& subState);

public:
	//Interface
	virtual bool UseStamina(float usedStamina) override;

	void StartWait() override;

	// callback
	virtual void Landed(const FHitResult& Hit);

	UFUNCTION()
	virtual void OntTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// INotifyState_ToCharacterReceiver interface
	void ReceiveNotification(EAnimNotifyToCharacterTypes curNotiType, bool bIsEnd) override;
	// End of INotifyState_ToCharacterReceiver interface


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

