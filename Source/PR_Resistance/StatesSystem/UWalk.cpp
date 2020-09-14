// Fill out your copyright notice in the Description page of Project Settings.


#include "UWalk.h"
#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"

UWalk::UWalk()
{
	mDesc.StateType = CharacterState::CS_WALK;
	mDesc.Priority = 2;
}

UWalk::~UWalk()
{
}

bool UWalk::_Init()
{
	void* buffer;
	bool result;
	result = GetCharacterDataArchive()->GetData("Status", &buffer);
	if (result)
	{
		mCharacterStatus = (FStatus*)buffer;
		result = GetCharacterDataArchive()->GetData("MovementSpeed", &buffer);
		if (result)
		{
			mMaxWalkSpeed = (float*)buffer;
			(*mMaxWalkSpeed) = mCharacterStatus->walkSpeed;
		}
	}
	return true;
}

bool UWalk::Begin(CharacterState prevState)
{
	
	return true;
}

void UWalk::Update(float deltaTime)
{
}

void UWalk::End()
{
}
