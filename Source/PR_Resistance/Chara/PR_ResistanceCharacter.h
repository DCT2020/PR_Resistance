// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "PR_Resistance/StatesSystem/Status.h"
//struct
#include "PR_Resistance/Combo/Action.h"
//Interface
#include "PR_Resistance/Interface/IStaminaProvider.h"
#include "PR_Resistance/ITimeToNextStepNotify.h"
//Components
#include "Components/StaticMeshComponent.h"

//
#include <functional>
//

#include "PR_ResistanceCharacter.generated.h"

// fornt declare
class StateManager_Player;

//Dynamic Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_OneParam, float, percent);

UCLASS(config=Game)
class APR_ResistanceCharacter : public ACharacter , public IStaminaProvider, public IITimeToNextStepNotify
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Rifle;


private:
	// ���� ������
	// combo
	ActionInput mLastInput = ActionInput::AINPUT_NULL;

	//state
	std::shared_ptr<StateManager_Player> mStateManager;


	std::function<void()> mTimeToNextStepNotifier;


public:
	APR_ResistanceCharacter();
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

	UPROPERTY(EditAnywhere, Category = Attack)
	UDataTable* mActionDataTable;

	UPROPERTY(BlueprintReadWrite, Category = Attack)
	bool bIsMeele = true;

	// TODO ������ ��, ���� �� �� �ӽú����� Archive�� �ø���� �߻��ϴ� ����(ȣ���Լ��� ������ ������ �����)
	FVector mLastInputVector = FVector::ZeroVector;

	// ���׹̳� ��� �̺�Ʈ ������(��������Ʈ)
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
		FDele_Dynamic_OneParam FucDynamicOneParam;

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



	// Jump
	void Jump_Wrapped();
	void StopJumpping_Wrapped();

	// Dodge
	void Dodge();

	// Jump Dash
	void DoJumpDash();

	//Attack
	void StartAttack();
	void StopAttack();

	// Swap
	void SetWeapon1();
	void SetWeapon2();


public:
	//Interface
	virtual bool UseStamina(float usedStamina) override;

	void StartWait() override;

	// callback
	virtual void Landed(const FHitResult& Hit);
	

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
