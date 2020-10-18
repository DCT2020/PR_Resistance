// Fill out your copyright notice in the Description page of Project Settings.

#include "UJumpDash.h"
#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"
#include "PR_Resistance/Interface/IStaminaProvider.h"
#include <cassert>

UJumpDash::UJumpDash()
{
	mDesc.StateType = (uint8)CharacterState::CS_JUMPDASH;
	mDesc.Priority = 5;
}

UJumpDash::~UJumpDash()
{
}


bool UJumpDash::_Init()
{
	return true;
}

bool UJumpDash::Begin(uint8 prevState)
{
	if (prevState != (uint8)CharacterState::CS_JUMP)
		return false;

	mElapsedTime = 0.0f;

	void* buffer = nullptr;
	
	GetCharaDataWithLog("Status",&buffer);
	{
		mCharacterStatus = (FStatus*)buffer;
		if (!mSPProvider->UseStamina(mCharacterStatus->maxStamina * mCharacterStatus->JumpDashStamina))
			return false;
	}

	GetCharaDataWithLog("CharacterGravityScale", &buffer);
	{
		mCharacterGScale = (float*)buffer;
		mOriginalGScale = (*mCharacterGScale);
		(*mCharacterGScale) = 0.0f;
	}

	GetCharaDataWithLog("MovementSpeed", &buffer);
	{
		mMaxWalkSpeed = (float*)buffer;
		(*mMaxWalkSpeed) = mCharacterStatus->JumpDashDistance / mCharacterStatus->JumpDashTime;
	}
	
	GetCharaDataWithLog("CharacterVelocity", &buffer);
	{
		mVelocity = (FVector*)buffer;
	}

	GetCharaDataWithLog("LastInputVector", &buffer);
	{
		mDirection = *(FVector*)buffer;
		if (mDirection.IsZero())
		{
			mDirection = FVector::ForwardVector;
		}
		(*mVelocity) = mDirection * (*mMaxWalkSpeed);
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
