// Fill out your copyright notice in the Description page of Project Settings.

#include "UJumpDash.h"
#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"
#include "PR_Resistance/Interface/IStaminaProvider.h"
#include <cassert>

UJumpDash::UJumpDash()
{
	mDesc.StateType = CharacterState::CS_JUMPDASH;
	mDesc.Priority = 5;
}

UJumpDash::~UJumpDash()
{
}


bool UJumpDash::_Init()
{
	return true;
}

bool UJumpDash::Begin(CharacterState prevState)
{
	if (prevState != CharacterState::CS_JUMP)
		return false;

	mElapsedTime = 0.0f;

	void* buffer = nullptr;
	bool result = true;
	result = GetCharacterDataArchive()->GetData("Status", &buffer);
	if (result)
	{
		mCharacterStatus = (FStatus*)buffer;
		if (!mSPProvider->UseStamina(mCharacterStatus->maxStamina * mCharacterStatus->JumpDashStamina))
			return false;
			
	}
	result = GetCharacterDataArchive()->GetData("CharacterGravityScale", &buffer);
	if (result)
	{
		mCharacterGScale = (float*)buffer;
		mOriginalGScale = (*mCharacterGScale);
		(*mCharacterGScale) = 0.0f;
	}
	result = GetCharacterDataArchive()->GetData("MovementSpeed", &buffer);
	if(result)
	{
		mMaxWalkSpeed = (float*)buffer;
		(*mMaxWalkSpeed) = mCharacterStatus->JumpDashDistance / mCharacterStatus->JumpDashTime;
	}
	result = GetCharacterDataArchive()->GetData("CharacterVelocity", &buffer);
	if (result)
	{
		mVelocity = (FVector*)buffer;
		result = GetCharacterDataArchive()->GetData("LastInputVector", &buffer);
		if (result)
		{
			mDirection = *(FVector*)buffer;
			if (mDirection.IsZero())
			{
				mDirection = FVector::ForwardVector;
			}
			(*mVelocity) = mDirection * (*mMaxWalkSpeed);
		}
	}
	

	assert(mSPProvider == nullptr);
		return true;
}

void UJumpDash::Update(float deltaTime)
{
	if (mElapsedTime >= mCharacterStatus->JumpDashTime)
	{
		mElapsedTime = 0.0f;
		mDesc.bIsEnd = true;
		(*mCharacterGScale) = mOriginalGScale;
		(*mMaxWalkSpeed) = mCharacterStatus->walkSpeed;
		(*mVelocity) = FVector::ZeroVector;
	}
	else
	{
		(*mVelocity) = mDirection * (*mMaxWalkSpeed);
		mElapsedTime += deltaTime;
	}
}

void UJumpDash::End()
{
}

void UJumpDash::SetProvider(IStaminaProvider* provider)
{
	assert(provider == nullptr);
	mSPProvider = provider;
}