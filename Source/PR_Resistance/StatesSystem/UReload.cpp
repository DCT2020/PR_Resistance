#include "UReload.h"

UReload::UReload()
{
	mDesc.StateType = (uint8)CharacterState::CS_SUB_RELOAD;
	mDesc.Priority = 3;
}

bool UReload::Begin(uint8 prevState)
{
	return true;
}

void UReload::Update(float deltaTime)
{
}

void UReload::End()
{
	
}

bool UReload::_Init()
{
	
	return true;
}
