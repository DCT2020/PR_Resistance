// Fill out your copyright notice in the Description page of Project Settings.


#include "URun.h"
#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"
#include "PR_Resistance/Interface/IStaminaProvider.h"
#include <cassert>

URun::URun()
{
	mDesc.StateType = (uint8)CharacterState::CS_RUN;
	mDesc.Priority = 3;
}

URun::~URun()
{
}

bool URun::_Init()
{
	void* buffer;

	GetCharaDataWithLog("Status", &buffer);
	mCharacterStatus = (FStatus*)buffer;

	GetCharaDataWithLog("MovementSpeed", &buffer);
	mMaxWalkSpeed = (float*)buffer;

	return true;
}

bool URun::Begin(uint8 prevState)
{
	if(prevState != (uint8)CharacterState::CS_WALK)
		return false;

	assert(mSPProvider == nullptr);
	(*mMaxWalkSpeed) = mCharacterStatus->runSpeed;

	return true;
}

void URun::Update(float deltaTime)
{
// 수정해야 할 것
	bool isCanRun = mSPProvider->UseStamina(mCharacterStatus->maxStamina * mSPUsePerSec * deltaTime);
	if(!isCanRun)
	{
		this->SetStop();
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
