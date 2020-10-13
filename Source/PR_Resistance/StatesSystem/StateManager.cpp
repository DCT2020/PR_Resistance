// Fill out your copyright notice in the Description page of Project Settings.


#include "StateManager.h"
#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"
#include "PR_Resistance/StatesSystem/IState.h"
#include "PR_Resistance/StatesSystem/UIdle.h"
#include "PR_Resistance/StatesSystem/UWalk.h"
#include "PR_Resistance/StatesSystem/URun.h"

StateManager::StateManager(int stateTypeNum)
{
	if(mCDArchive == nullptr)
		mCDArchive = new CharacterDataArchive;

	mStateContiners.Reserve(stateTypeNum);
}

StateManager::~StateManager()
{
	mStates.Empty();
	if(mCDArchive != nullptr)
		delete mCDArchive;
}

bool StateManager::Init()
{
	return true;
}

void StateManager::TryChangeState(CharacterState stateType)
{
	// 이후 editor에서 가져오는걸로 바꾸기 (FStateDesc를)
	mStateChangeCalls.Enqueue(mStates[stateType]->GetStateDesc());
}

void StateManager::SetStateEnd(CharacterState stateType)
{
	mStates[stateType]->SetStop();
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

		if (mCurState->GetStateDesc().StateType != nextDesc.StateType)
		{
			if (mCurState->GetStateDesc().bIsEnd)
			{
				ChangeState(mStates[nextDesc.StateType]);
			}
			else if (nextDesc.Priority > mCurState->GetStateDesc().Priority)
			{
				ChangeState(mStates[nextDesc.StateType]);
			}
		}
	}

	if (mCurState != nullptr)
	{
		mCurState->Update(deltaTime);
	}
}

FStateDesc StateManager::GetCurStateDesc()
{
	return mCurState->GetStateDesc();
}

bool StateManager::AddArchiveData(FName key, void* data)
{
	return mCDArchive->AddData(key,data);
}

void StateManager::RemoveArchiveData(FName key)
{
	mCDArchive->RemoveData(key);
}

void StateManager::SetDefaultState(CharacterState state)
{
	mCurState = mStates[state];
}

void StateManager::ChangeStateContainer(int index)
{
	mStates = mStateContiners[index];
}

std::shared_ptr<IState> StateManager::AddStateData(int index, CharacterState stateName, std::shared_ptr<IState> newState)
{
	//for (int i = mStateContiners.Num(); i <= index; ++i)
	//{
	//	mStateContiners.Push(TMap<CharacterState, std::shared_ptr<IState>>());
	//}

	newState->Init(mCDArchive);
	if (!mStateContiners.IsValidIndex(index))
	{
		mStateContiners.EmplaceAt(index);
	}
	mStateContiners[index].Add(stateName,newState);
	return newState;
}

std::shared_ptr<IState> StateManager::GetStateData(int index, CharacterState stateName)
{
	return mStateContiners[index][stateName];
}

/*
*
*/
bool StateManager::ChangeState(std::shared_ptr<IState> newState)
{
	if(newState->Begin(mCurState->GetStateDesc().StateType))
	{
		mCurState->SetStop();
		mCurState->End();
		newState->SetStart();
		mCurState = newState;

		return true;
	}

	return false;
}
