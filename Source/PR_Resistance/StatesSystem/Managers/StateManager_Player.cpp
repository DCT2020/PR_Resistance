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
UStateManager_Player::UStateManager_Player()
{
}

UStateManager_Player::~UStateManager_Player()
{
	
}

bool UStateManager_Player::Init()
{
	UStateManager::Init();
	mSubState = NewObject<UStateManager_SubState>();
	mSubState->Init_SubState(GetCharacterDataArchive());

	// subState Init
	mSubState->Init();

	return true;
}

void UStateManager_Player::LoadStates()
{
	// ST_SWORD
#pragma region ST_SWORD
	AddStateData((uint8)StateType::ST_SWORD, CharacterState::CS_IDLE, NewObject<UIdle>());
	AddStateData((uint8)StateType::ST_SWORD, CharacterState::CS_WALK, NewObject<UWalk>());

	auto run = NewObject<URun>();
	AddStateData((uint8)StateType::ST_SWORD, CharacterState::CS_RUN, run);
	run->SetProvider(mSPProvider);

	auto dodge = NewObject<UDodge>();
	AddStateData((uint8)StateType::ST_SWORD, CharacterState::CS_DODGE, dodge);
	dodge->SetProvider(mSPProvider);

	auto jumpDash = NewObject<UJumpDash>();
	AddStateData((uint8)StateType::ST_SWORD, CharacterState::CS_JUMPDASH, jumpDash);
	jumpDash->SetProvider(mSPProvider);

	AddStateData((uint8)StateType::ST_SWORD, CharacterState::CS_JUMP, NewObject<UJump>());

	auto attack = NewObject<UAttack>();
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
	AddStateData((uint8)StateType::ST_GUN, CharacterState::CS_ATTACK, NewObject<UFire>());
#pragma endregion

	SetDefaultState((uint8)StateType::ST_GUN, CharacterState::CS_IDLE);

	mSubState->LoadStates();
}


void UStateManager_Player::Update(float deltaTime)
{
	UStateManager::Update(deltaTime);
	mSubState->Update(deltaTime);

	TryChangeState(CharacterState::CS_IDLE);
}

bool UStateManager_Player::UseStamina(float usedStamina)
{
	assert(mSPProvider == nullptr);
	return mSPProvider->UseStamina(usedStamina);
}

void UStateManager_Player::SetProvider(IStaminaProvider* provider)
{
	assert(provider == nullptr);
	mSPProvider = provider;
}

void UStateManager_Player::ChangeState(StateType type)
{	
	mCurStateType = type;
	ChangeStateContainer((uint8)mCurStateType);
}

CharacterState UStateManager_Player::GetCurSubState()
{
	return mSubState->GetCurStateDesc().StateType;
}

void UStateManager_Player::TryChangeSubState(CharacterState subState)
{
	mSubState->TryChangeState(subState);
}

void UStateManager_Player::SetSubStateEnd(CharacterState subState)
{
	mSubState->SetStateEnd(subState);
}