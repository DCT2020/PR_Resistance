// Fill out your copyright notice in the Description page of Project Settings.


#include "StateManager_Player.h"
#include "PR_Resistance/StatesSystem/UIdle.h"
#include "PR_Resistance/StatesSystem/UWalk.h"
#include "PR_Resistance/StatesSystem/URun.h"
#include "PR_Resistance/StatesSystem/UJump.h"
#include "PR_Resistance/StatesSystem/UDodge.h"
#include "PR_Resistance/StatesSystem/UJumpDash.h"
#include "PR_Resistance/StatesSystem/UAttack.h"
#include "PR_Resistance/StatesSystem/Fire.h"

#include "PR_Resistance/Interface/IStaminaUser.h"

#include <cassert>
StateManager_Player::StateManager_Player(int stateTypeNum)
	:StateManager(stateTypeNum)
{
}

StateManager_Player::~StateManager_Player()
{
}

bool StateManager_Player::Init()
{

	// ST_SWORD
	#pragma region ST_SWORD
		AddStateData((uint8)StateType::ST_SWORD, CharacterState::CS_IDLE, std::make_shared<UIdle>());
		AddStateData((uint8)StateType::ST_SWORD, CharacterState::CS_WALK, std::make_shared<UWalk>());

		auto run = std::make_shared<URun>();
		AddStateData((uint8)StateType::ST_SWORD, CharacterState::CS_RUN, run);
		run->SetProvider(mSPProvider);

		auto dodge = std::make_shared<UDodge>();
		AddStateData((uint8)StateType::ST_SWORD, CharacterState::CS_DODGE, dodge);
		dodge->SetProvider(mSPProvider);

		auto jumpDash = std::make_shared<UJumpDash>();
		AddStateData((uint8)StateType::ST_SWORD, CharacterState::CS_JUMPDASH, jumpDash);
		jumpDash->SetProvider(mSPProvider);

		AddStateData((uint8)StateType::ST_SWORD, CharacterState::CS_JUMP, std::make_shared<UJump>());

		auto attack = std::make_shared<UAttack>();
		AddStateData((uint8)StateType::ST_SWORD, CharacterState::CS_ATTACK, attack);
		attack->SetProvider(mSPProvider);
	#pragma endregion

	#pragma region ST_GUN
		auto temp = GetStateData((uint8)StateType::ST_SWORD, CharacterState::CS_IDLE);
		AddStateData((uint8)StateType::ST_GUN, CharacterState::CS_IDLE, temp);

		temp = GetStateData((uint8)StateType::ST_SWORD, CharacterState::CS_WALK);
		AddStateData((uint8)StateType::ST_GUN, CharacterState::CS_WALK, temp);

		temp = GetStateData((uint8)StateType::ST_SWORD, CharacterState::CS_JUMP);
		AddStateData((uint8)StateType::ST_GUN, CharacterState::CS_JUMP, temp);

		AddStateData((uint8)StateType::ST_GUN, CharacterState::CS_RUN, run);
		AddStateData((uint8)StateType::ST_GUN, CharacterState::CS_DODGE, dodge);
		AddStateData((uint8)StateType::ST_GUN, CharacterState::CS_JUMPDASH, jumpDash);
		AddStateData((uint8)StateType::ST_GUN, CharacterState::CS_ATTACK, std::make_shared<Fire>());

	#pragma endregion
	

	ChangeStateContainer((uint8)StateType::ST_GUN);
	SetDefaultState(CharacterState::CS_IDLE);

	return true;
}

void StateManager_Player::Update(float deltaTime)
{
	StateManager::Update(deltaTime);
	mChildStateManager.Update(deltaTime);
}

bool StateManager_Player::UseStamina(float usedStamina)
{
	assert(mSPProvider == nullptr);
	return mSPProvider->UseStamina(usedStamina);
}

void StateManager_Player::SetProvider(IStaminaProvider* provider)
{
	assert(provider == nullptr);
	mSPProvider = provider;
}

void StateManager_Player::ChangeState(StateType type)
{	
	mCurStateType = type;
	ChangeStateContainer((uint8)mCurStateType);
}

CharacterState StateManager_Player::GetChildState()
{
	return mChildStateManager.GetCurStateDesc().StateType;
}
