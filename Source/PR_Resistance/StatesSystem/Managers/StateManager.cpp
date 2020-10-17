// Fill out your copyright notice in the Description page of Project Settings.


#include "StateManager.h"

UStateManager::UStateManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.bAllowTickOnDedicatedServer
}


UStateManager::~UStateManager()
{
}

bool UStateManager::Init()
{
	mCDArchive = NewObject<UCharacterDataArchive>();
	return true;
}

void UStateManager::TryChangeState(CharacterState stateType)
{
	// 이후 editor에서 가져오는걸로 바꾸기 (FStateDesc를)
	mStateChangeCalls.Enqueue(mStates.mStateContainer[stateType]->GetStateDesc());
}

void UStateManager::SetStateEnd(CharacterState stateType)
{
	mStates.mStateContainer[stateType]->SetStop();
}

void UStateManager::SetState(CharacterState stateType)
{
	ChangeState(mStates.mStateContainer[stateType]);
}

void UStateManager::Update(float deltaTime)
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
				ChangeState(mStates.mStateContainer[nextDesc.StateType]);
			}
			else if (nextDesc.Priority >= mCurState->GetStateDesc().Priority)
			{
				ChangeState(mStates.mStateContainer[nextDesc.StateType]);
			}
		}
	}

	if (mCurState != nullptr)
	{
		mCurState->Update(deltaTime);
	}
}

FStateDesc UStateManager::GetCurStateDesc()
{
	return mCurState->GetStateDesc();
}

bool UStateManager::AddArchiveData(FName key, void* data)
{
	return mCDArchive->AddData(key,data);
}

void UStateManager::RemoveArchiveData(FName key)
{
	mCDArchive->RemoveData(key);
}

UStateManager::FChangeEvent& UStateManager::OnStateChange()
{
	return mChangeEvent;
}

void UStateManager::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	this->Update(DeltaTime);
}

UCharacterDataArchive * UStateManager::GetCharacterDataArchive()
{
	return mCDArchive;
}

void UStateManager::SetCharacterDataArchive(UCharacterDataArchive* archive)
{
	mCDArchive = archive;
}

void UStateManager::SetDefaultState(int index,CharacterState state)
{
	ChangeStateContainer(index);
	mCurState = mStates.mStateContainer[state];
}

void UStateManager::ChangeStateContainer(int index)
{
	mStates = mStateContiners[index];
}

UCState* UStateManager::AddStateData(int index, CharacterState stateName, UCState* newState)
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
	mStateContiners[index].mStateContainer.Add(stateName,newState);

	return newState;
}

UCState* UStateManager::GetStateData(int index, CharacterState stateName)
{
	return mStateContiners[index].mStateContainer[stateName];
}

/*
*
*/
bool UStateManager::ChangeState(UCState* newState)
{
	if(newState->Begin(mCurState->GetStateDesc().StateType))
	{
		//broadcast
		mChangeEvent.Broadcast(mCurState->GetStateDesc().StateType, newState->GetStateDesc().StateType);

		mCurState->SetStop();
		mCurState->End();
		newState->SetStart();
		mCurState = newState;

		return true;
	}

	return false;
}

