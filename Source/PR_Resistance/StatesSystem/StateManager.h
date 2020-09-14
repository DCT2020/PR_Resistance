// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/StateDesc.h"


class IState;
class CharacterDataArchive;
/**
 * 
 */
class PR_RESISTANCE_API StateManager
{
private:
	TQueue<FStateDesc> mStateChangeCalls;
	TMap<CharacterState,std::shared_ptr<IState>> mStates; // State����Ʈ�� �����Ϳ��� ������ �ֵ���
	std::shared_ptr<IState> mCurState = nullptr;
	CharacterDataArchive* mCDArchive = nullptr;
public:
	StateManager();
	virtual ~StateManager();

	/*
	* Init ȣ�� ������ AddArchiveData�� ȣ��Ǿ�� �մϴ�.
	*/
	virtual bool Init() = 0;
	/*
	* State������ �õ��մϴ�.
	* FStateDesc�� Priority�� ���� �����ϰų� �����մϴ�.
	* ���� �����ӿ� �õ��˴ϴ�.
	*/
	void TryChangeState(CharacterState stateType);
	/*
	* ���� ���¸� �����ŵ�ϴ�.
	*/
	void SetStateEnd(CharacterState stateType);
	/*
	* Priroty�� �����ϰ� ��� ���¸� �����մϴ�.
	*/
	void SetState(CharacterState destateTypesc);
	void Update(float deltaTime);
	FStateDesc GetCurStateDesc();

	/*
	* key : �����͸� ã���� ���� Ű���Դϴ�.
	* data : ������ �������Դϴ�. �������� �����ֱ�� �Լ��� ȣ���� ��ü�� å�������մϴ�.
	* return : ������ true, ���н� false�� �����մϴ�.
	*/
	bool AddArchiveData(FName key, void* data);
	void RemoveArchiveData(FName key);

protected:

	void SetDefaultState(CharacterState state);
	std::shared_ptr<IState>  AddStateData(CharacterState stateName, std::shared_ptr<IState> newState);
	bool ChangeState(std::shared_ptr<IState> newState);
};
