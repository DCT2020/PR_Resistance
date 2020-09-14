// Fill out your copyright notice in the Description page of Project Settings.


#include "StateManager_Player.h"
#include "PR_Resistance/StatesSystem/UIdle.h"
#include "PR_Resistance/StatesSystem/UWalk.h"
#include "PR_Resistance/StatesSystem/URun.h"
#include "PR_Resistance/StatesSystem/UJump.h"
#include "PR_Resistance/StatesSystem/UDodge.h"
#include "PR_Resistance/StatesSystem/UJumpDash.h"

#include "PR_Resistance/Interface/IStaminaUser.h"

#include <cassert>
StateManager_Player::StateManager_Player()
{
}

StateManager_Player::~StateManager_Player()
{
}

bool StateManager_Player::Init()
{
	AddStateData(CharacterState::CS_IDLE, std::make_shared<UIdle>());
	AddStateData(CharacterState::CS_WALK, std::make_shared<UWalk>());

	auto run = std::make_shared<URun>();
	AddStateData(CharacterState::CS_RUN, run);
	run->SetProvider(mSPProvider);
	
	auto dodge = std::make_shared<UDodge>();
	AddStateData(CharacterState::CS_DODGE, dodge);
	dodge->SetProvider(mSPProvider);

	auto jumpDash = std::make_shared<UJumpDash>();
	AddStateData(CharacterState::CS_JUMPDASH, jumpDash);
	jumpDash->SetProvider(mSPProvider);

	AddStateData(CharacterState::CS_JUMP, std::make_shared<UJump>());

	SetDefaultState(CharacterState::CS_IDLE);

	return true;
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
