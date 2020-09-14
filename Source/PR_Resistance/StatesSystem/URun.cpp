// Fill out your copyright notice in the Description page of Project Settings.


#include "URun.h"
#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"
#include "PR_Resistance/Interface/IStaminaProvider.h"
#include <cassert>

URun::URun()
{
	mDesc.StateType = CharacterState::CS_RUN;
	mDesc.Priority = 3;
}

URun::~URun()
{
}

bool URun::_Init()
{
	bool result = true;
	void* buffer;

	result = GetCharacterDataArchive()->GetData("Status", &buffer);
	if (result)
	{
		mCharacterStatus = (FStatus*)buffer;
		result = GetCharacterDataArchive()->GetData("MovementSpeed", &buffer);
		if (result)
		{
			mMaxWalkSpeed = (float*)buffer;
			(*mMaxWalkSpeed) = mCharacterStatus->runSpeed;
		}
	}

	return true;
}

bool URun::Begin(CharacterState prevState)
{
	assert(mSPProvider == nullptr);

	return true;
}

void URun::Update(float deltaTime)
{
	bool isCanRun = mSPProvider->UseStamina(mCharacterStatus->maxStamina * mSPUsePerSec * deltaTime);
	if(isCanRun)
	{
		(*mMaxWalkSpeed) = mCharacterStatus->runSpeed;
	}
	else
	{
		(*mMaxWalkSpeed) = mCharacterStatus->walkSpeed;
	}
}

void URun::End()
{
	(*mMaxWalkSpeed) = mCharacterStatus->walkSpeed;
}

//interface
void URun::SetProvider(IStaminaProvider* provider)
{
	assert(provider == nullptr);
	mSPProvider = provider;
}
