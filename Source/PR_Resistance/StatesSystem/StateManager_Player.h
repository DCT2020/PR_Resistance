// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/StateManager.h"

// interface
#include "PR_Resistance/Interface/IStaminaProvider.h"
#include "PR_Resistance/Interface/IStaminaUser.h"
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

class PR_RESISTANCE_API StateManager_Player : 
	public StateManager, public IStaminaProvider, public IStaminaUser
{
protected:
	StateManager mChildStateManager;

	IStaminaProvider* mSPProvider = nullptr;
	StateType mCurStateType = StateType::ST_SWORD;
public:
	StateManager_Player(int stateTypeNum);
	virtual ~StateManager_Player() override;


	bool Init() override;
	void Update(float deltaTime) override;
	bool UseStamina(float usedStamina) override;
	void SetProvider(IStaminaProvider* provider) override;

	void ChangeState(StateType type);

	CharacterState GetChildState();
};
