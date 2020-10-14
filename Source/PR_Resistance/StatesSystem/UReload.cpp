#include "UReload.h"

UReload::UReload()
{
	mDesc.StateType = CharacterState::CS_SUB_RELOAD;
	mDesc.Priority = 1;
}

bool UReload::Begin(CharacterState prevState)
{
	elapsedTime = 0.0f;
	return true;
}

void UReload::Update(float deltaTime)
{
	elapsedTime+= deltaTime	;
	if (elapsedTime >= 2.0f)
	{
		elapsedTime = 0.0f;
		mDesc.bIsEnd = true;
	}
}

void UReload::End()
{
}

bool UReload::_Init()
{
	return true;
}
