// Fill out your copyright notice in the Description page of Project Settings.


#include "StateManager.h"
#include "PR_Resistance/StatesSystem/IState.h"
#include "PR_Resistance/StatesSystem/UIdle.h"
#include "PR_Resistance/StatesSystem/UWalk.h"

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
	mStates.Empty();
}

bool StateManager::Init()
{
	mStates.Add(MakeTuple<CharacterState, std::shared_ptr<IState>>(CharacterState::CS_IDLE, std::shared_ptr<IState>(new UIdle)));
	mStates.Add(MakeTuple<CharacterState, std::shared_ptr<IState>>(CharacterState::CS_WALK, std::shared_ptr<IState>(new UWalk)));

	mCurState = mStates[CharacterState::CS_IDLE];
	return true;
}

void StateManager::TryChangeState(CharacterState stateType)
{
	// 이후 editor에서 가져오는걸로 바꾸기 (FStateDesc를)
	mStateChangeCalls.Enqueue(mStates[stateType]->GetDesc());
}

void StateManager::SetState(CharacterState stateType)
{
	ChangeState(mStates[stateType]);
}

void StateManager::Update(float deltaTime)
{
	if (!mStateChangeCalls.IsEmpty())
	{
		FStateDesc curDesc;
		FStateDesc nextDesc;
		while (mStateChangeCalls.Dequeue(curDesc))
		{
			if (nextDesc.StateType == CharacterState::CS_NULL)
				nextDesc = curDesc;
			else if (curDesc.Priority > nextDesc.Priority)
				nextDesc = curDesc;
		}

		if (!mCurState->GetDesc().bIsManualStop)
		{
			ChangeState(mStates[nextDesc.StateType]);
		}
		
	}

	if(mCurState != nullptr)
		mCurState->Update(deltaTime);
}

FStateDesc StateManager::GetCurStateDesc()
{
	return mCurState->GetDesc();
}

/*
*
*/
bool StateManager::ChangeState(std::shared_ptr<IState> newState)
{
	mCurState->End();
	newState->Begin();
	mCurState = newState;

	return true;
}
