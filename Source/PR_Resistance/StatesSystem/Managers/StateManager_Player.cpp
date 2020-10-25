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
#include "PR_Resistance/Hit.h"

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

	TBaseDelegate<void,uint8,uint8> delegator;
	delegator.BindUObject(this, &UStateManager_Player::OnSubStateChange);
	mSubState->BindStateChangeCall(delegator);

	delegator.BindUObject(this, &UStateManager_Player::OnMyStateChange);
	this->BindStateChangeCall(delegator);


	return true;
}

void UStateManager_Player::LoadStates()
{
	// ST_SWORD
#pragma region ST_SWORD
	AddStateData((uint8)StateType::ST_SWORD, (uint8)CharacterState::CS_IDLE, NewObject<UIdle>());
	AddStateData((uint8)StateType::ST_SWORD, (uint8)CharacterState::CS_WALK, NewObject<UWalk>());

	auto run = NewObject<URun>();
	run->SetProvider(mSPProvider);
	AddStateData((uint8)StateType::ST_SWORD, (uint8)CharacterState::CS_RUN, run);

	auto dodge = NewObject<UDodge>();
	dodge->SetProvider(mSPProvider);
	AddStateData((uint8)StateType::ST_SWORD, (uint8)CharacterState::CS_DODGE, dodge);

	auto jumpDash = NewObject<UJumpDash>();
	jumpDash->SetProvider(mSPProvider);
	AddStateData((uint8)StateType::ST_SWORD, (uint8)CharacterState::CS_JUMPDASH, jumpDash);

	AddStateData((uint8)StateType::ST_SWORD, (uint8)CharacterState::CS_JUMP, NewObject<UJump>());

	auto attack = NewObject<UAttack>();
	attack->SetProvider(mSPProvider);
	AddStateData((uint8)StateType::ST_SWORD, (uint8)CharacterState::CS_ATTACK, attack);

	auto hit = NewObject<UHit>();
	AddStateData((uint8)StateType::ST_SWORD, (uint8)CharacterState::CS_HIT, hit);

	
#pragma endregion

#pragma region ST_GUN
	auto temp = GetStateData((uint8)StateType::ST_SWORD, (uint8)CharacterState::CS_IDLE);
	AddStateData((uint8)StateType::ST_GUN, (uint8)CharacterState::CS_IDLE, temp,false);

	temp = GetStateData((uint8)StateType::ST_SWORD, (uint8)CharacterState::CS_WALK);
	AddStateData((uint8)StateType::ST_GUN, (uint8)CharacterState::CS_WALK, temp, false);

	temp = GetStateData((uint8)StateType::ST_SWORD, (uint8)CharacterState::CS_JUMP);
	AddStateData((uint8)StateType::ST_GUN, (uint8)CharacterState::CS_JUMP, temp, false);

	AddStateData((uint8)StateType::ST_GUN, (uint8)CharacterState::CS_RUN, run, false);
	AddStateData((uint8)StateType::ST_GUN, (uint8)CharacterState::CS_DODGE, dodge, false);
	AddStateData((uint8)StateType::ST_GUN, (uint8)CharacterState::CS_JUMPDASH, jumpDash, false);

	AddPlayerBaseState((uint8)StateType::ST_GUN, CharacterState::CS_ATTACK, NewObject<UFire>());

	AddStateData((uint8)StateType::ST_GUN, (uint8)CharacterState::CS_HIT, hit);
#pragma endregion

	SetDefaultState((uint8)StateType::ST_GUN, (uint8)CharacterState::CS_IDLE);

	mSubState->LoadStates();
}


void UStateManager_Player::AddPlayerBaseState(int index, CharacterState stateName, UCState_PlayerBase* newState)
{
	newState->InitDatas(&mCurStateInfo);
	AddStateData(index, (uint8)stateName, newState);
}

void UStateManager_Player::Update(float deltaTime)
{
	if (GetOwnerRole() != ROLE_Authority)
		return;

	UStateManager::Update(deltaTime);
	mSubState->Update(deltaTime);

	TryChangeState((uint8)CharacterState::CS_IDLE);

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
	return (CharacterState)mSubState->GetCurStateDesc().StateType;
}

void UStateManager_Player::TryChangeSubState(CharacterState subState)
{
	if (GetCurStateDesc().StateType == (uint8)CharacterState::CS_DODGE)
		return;
	
	mSubState->TryChangeState((uint8)subState);
}

void UStateManager_Player::SetSubStateEnd(CharacterState subState) const
{
	mSubState->SetStateEnd((uint8)subState);
}


// binded function
void UStateManager_Player::OnSubStateChange(uint8 prevState, uint8 newState)
{
	mCurStateInfo.mCurSubState = (CharacterState)newState;
}

void UStateManager_Player::OnMyStateChange(uint8 prevState, uint8 newState)
{
	mCurStateInfo.mCurMainState = (CharacterState)newState;
}

void UStateManager_Player::TryChangeState(uint8 stateType)
{
	if(stateType == (uint8)CharacterState::CS_DODGE)
	{
		mSubState->SetStateEnd(mSubState->GetCurStateDesc().StateType);
	}
	UStateManager::TryChangeState(stateType);
}
