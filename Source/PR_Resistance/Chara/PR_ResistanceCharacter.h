// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PR_Resistance/StatesSystem/Status.h"
//Interface
#include "PR_Resistance/Interface/IStaminaProvider.h"
//
#include "PR_ResistanceCharacter.generated.h"

// fornt declare
class StateManager_Player;

//Dynamic Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_OneParam, float, percent);

UCLASS(config=Game)
class APR_ResistanceCharacter : public ACharacter , public IStaminaProvider
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	APR_ResistanceCharacter();
	virtual ~APR_ResistanceCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//States
	UPROPERTY(EditAnywhere, Category = Character)
	FStatus mStatus;

	std::shared_ptr<StateManager_Player> mStateManager;

	// TODO 수정할 것, 리턴 값 즉 임시변수를 Archive에 올릴경우 발생하는 문제(호출함수가 끝나면 변수가 사라짐)
	FVector mLastInputVector = FVector::ZeroVector;

	virtual void Landed(const FHitResult& Hit);

protected: 
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

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
	void Run();
	void RunStop();

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// 스테미나 사용 이벤트 디스패쳐(델리게이트)
	UPROPERTY(BlueprintAssignable, VisibleAnywhere,BlueprintCallable, Category = "Event")
		FDele_Dynamic_OneParam FucDynamicOneParam;

	// Jump
	void Jump_Wrapped();
	void StopJumpping_Wrapped();

	// Dodge
	void Dodge();

	// Jump Dash
	void DoJumpDash();

public:
	//Interface
	virtual bool UseStamina(float usedStamina) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

