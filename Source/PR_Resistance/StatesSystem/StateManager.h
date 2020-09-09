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
	TMap<CharacterState,std::shared_ptr<IState>> mStates; // State����Ʈ�� �����Ϳ��� ������ �ֵ���
	std::shared_ptr<IState> mCurState = nullptr;
public:
	StateManager();
	~StateManager();

	
	bool Init();
	/*
	* State������ �õ��մϴ�.
	* FStateDesc�� Priority�� ���� �����ϰų� �����մϴ�.
	* ���� �����ӿ� �õ��˴ϴ�.
	*/
	void TryChangeState(CharacterState stateType);
	/*
	* Priroty�� �����ϰ� ��� ���¸� �����մϴ�.
	*/
	void SetState(CharacterState destateTypesc);
	void Update(float deltaTime);
	FStateDesc GetCurStateDesc();

private:

	bool ChangeState(std::shared_ptr<IState> newState);
};
