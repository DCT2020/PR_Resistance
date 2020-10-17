#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "StateManager.h"
#include "StateManager_SubState.generated.h"

UCLASS()
class UStateManager_SubState : public UStateManager
{
	GENERATED_BODY()
public:
	UStateManager_SubState();

	// StateManager
	bool Init_SubState(UCharacterDataArchive* archive);
	void Update(float deltaTime) override;

	void LoadStates() override;

};