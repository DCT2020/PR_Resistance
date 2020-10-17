#include "StateManager_SubState.h"

// States
#include "PR_Resistance/StatesSystem/UReload.h"
#include "PR_Resistance/StatesSystem/UIdle.h"


UStateManager_SubState::UStateManager_SubState()
{
	
}

bool UStateManager_SubState::Init_SubState(UCharacterDataArchive* archive)
{
	SetCharacterDataArchive(archive);
	return true;
}

void UStateManager_SubState::Update(float deltaTime)
{
	UStateManager::Update(deltaTime);
	TryChangeState(CharacterState::CS_IDLE);
}

void UStateManager_SubState::LoadStates()
{
	AddStateData(0, CharacterState::CS_IDLE, NewObject<UIdle>());
	AddStateData(0, CharacterState::CS_SUB_RELOAD, NewObject<UReload>());

	SetDefaultState(0, CharacterState::CS_IDLE);
}
