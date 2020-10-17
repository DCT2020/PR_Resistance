#include "UReload.h"

UReload::UReload()
{
	mDesc.StateType = CharacterState::CS_SUB_RELOAD;
	mDesc.Priority = 2;
}

bool UReload::Begin(CharacterState prevState)
{
	elapsedTime = 0.0f;
	return true;
}

void UReload::Update(float deltaTime)
{
	elapsedTime+= deltaTime	;
	if (elapsedTime >= 3.1f)
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