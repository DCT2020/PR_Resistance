// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/StateDesc.h"

class IState;
/**
 * 
 */
class PR_RESISTANCE_API StateManager
{
private:
	TQueue<FStateDesc> mStateChangeCalls;
	TMap<CharacterState,std::shared_ptr<IState>> mStates; // State리스트는 에디터에서 받을수 있도록
	std::shared_ptr<IState> mCurState = nullptr;
public:
	StateManager();
	~StateManager();

	
	bool Init();
	/*
	* State변경을 시도합니다.
	* FStateDesc의 Priority에 따라 실패하거나 성공합니다.
	* 다음 프레임에 시도됩니다.
	*/
	void TryChangeState(CharacterState stateType);
	/*
	* Priroty를 무시하고 즉시 상태를 변경합니다.
	*/
	void SetState(CharacterState destateTypesc);
	void Update(float deltaTime);
	FStateDesc GetCurStateDesc();

private:

	bool ChangeState(std::shared_ptr<IState> newState);
};
