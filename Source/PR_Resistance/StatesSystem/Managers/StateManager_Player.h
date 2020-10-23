// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "PR_Resistance/StatesSystem/Managers/StateManager.h"

//my class
#include "PR_Resistance/StatesSystem/Managers/StateManager_SubState.h"
#include "PR_Resistance/StatesSystem/CState_PlayerBase.h"

// interface
#include "PR_Resistance/Interface/IStaminaProvider.h"
#include "PR_Resistance/Interface/IStaminaUser.h"

#include "StateManager_Player.generated.h"
/**
 * 
 */
 UENUM(BlueprintType)
enum class StateType : uint8
{
	ST_SWORD,
	ST_GUN,
	ST_MAX
 };

 UCLASS()
class PR_RESISTANCE_API UStateManager_Player final : 
	public UStateManager, public IStaminaProvider, public IStaminaUser
{
	GENERATED_BODY()
protected:
	IStaminaProvider* mSPProvider = nullptr;

	StateType mCurStateType = StateType::ST_SWORD;

	UPROPERTY();
	UStateManager_SubState* mSubState = nullptr;

	FStateData_PlayerBase mCurStateInfo;

public:
	UStateManager_Player();
	virtual ~UStateManager_Player() override;

	bool Init() override;
	void Update(float deltaTime) override;
	bool UseStamina(float usedStamina) override;
	void SetProvider(IStaminaProvider* provider) override;

	void ChangeState(StateType type);

	// subState
	CharacterState GetCurSubState();
	void TryChangeSubState(CharacterState subState);
	void SetSubStateEnd(CharacterState subState) const;

	void LoadStates() override;


protected:
	void AddPlayerBaseState(int index, CharacterState stateName, UCState_PlayerBase* newState);

	UFUNCTION()
		void OnSubStateChange(uint8 prevState, uint8 newState);

	UFUNCTION()
		void OnMyStateChange(uint8 prevState, uint8 newState);
public:
	virtual void TryChangeState(uint8 stateType) override;
 };
