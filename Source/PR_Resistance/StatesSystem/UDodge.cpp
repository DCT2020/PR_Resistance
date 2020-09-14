// Fill out your copyright notice in the Description page of Project Settings.


#include "UDodge.h"
#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"
#include "PR_Resistance/Interface/IStaminaProvider.h"

#include <cassert>

UDodge::UDodge()
{
	mDesc.StateType = CharacterState::CS_DODGE;
	mDesc.Priority = 5;
}

UDodge::~UDodge()
{
}

bool UDodge::_Init()
{
	return true;
}


bool UDodge::Begin(CharacterState prevState)
{
	assert(mSPProvider == nullptr);

	void* buffer = nullptr;
	if (GetCharacterDataArchive()->GetData("Status", &buffer))
	{
		mChracterStatus = (FStatus*)buffer;
		if (mSPProvider->UseStamina(mChracterStatus->maxStamina * mChracterStatus->DodgeStamina))
		{
			mElapsedTime = 0.0f;
			if (GetCharacterDataArchive()->GetData("MovementSpeed", &buffer))
			{
				mMaxWalkSpeed = (float*)buffer;
				(*mMaxWalkSpeed) = mChracterStatus->DodgeDistance / mChracterStatus->DodgeTime;
			}

			if (GetCharacterDataArchive()->GetData("CharacterVelocity", &buffer))
			{
				FVector* velocity = (FVector*)buffer;
				if (GetCharacterDataArchive()->GetData("LastInputVector", &buffer))
				{
					FVector* lastInputVector = (FVector*)buffer;
					(*velocity) = (*lastInputVector) * (*mMaxWalkSpeed);
				}
			}
		}
		else
		{
			mDesc.bIsEnd = true;
		}
	}

	return true;
}

void UDodge::Update(float deltaTime)
{
	if (mElapsedTime >= mChracterStatus->DodgeTime)
	{
		(*mMaxWalkSpeed) = mChracterStatus->walkSpeed;
		mElapsedTime = 0.0f;
		mDesc.bIsEnd = true;
	}
	else
	{
		mElapsedTime+= deltaTime;
	}
}

void UDodge::End()
{
}

void UDodge::SetProvider(IStaminaProvider* provider)
{
	assert(provider == nullptr);
	mSPProvider = provider;
}
