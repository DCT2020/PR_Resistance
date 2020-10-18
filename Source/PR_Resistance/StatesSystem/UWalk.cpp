// Fill out your copyright notice in the Description page of Project Settings.


#include "UWalk.h"
#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"

UWalk::UWalk()
{
	mDesc.StateType = (uint8)CharacterState::CS_WALK;
	mDesc.Priority = 2;
}


UWalk::~UWalk()
{

}

bool UWalk::_Init()
{
	void* buffer;

	GetCharaDataWithLog("Status", &buffer);
	{
		mCharacterStatus = (FStatus*)buffer;
	}

	GetCharaDataWithLog("MovementSpeed", &buffer);
	{
		mMaxWalkSpeed = (float*)buffer;
		(*mMaxWalkSpeed) = mCharacterStatus->walkSpeed;
	}

	return true;
}

bool UWalk::Begin(uint8 prevState)
{
	(*mMaxWalkSpeed) = mCharacterStatus->walkSpeed;
	return true;
}

void UWalk::Update(float deltaTime)
{
}

void UWalk::End()
{
}
