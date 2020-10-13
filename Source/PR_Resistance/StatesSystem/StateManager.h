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
	TArray<TMap<CharacterState, std::shared_ptr<IState>>> mStateContiners;
	TMap<CharacterState,std::shared_ptr<IState>> mStates; // State����Ʈ�� �����Ϳ��� ������ �ֵ���
	std::shared_ptr<IState> mCurState = nullptr;
	CharacterDataArchive* mCDArchive = nullptr;
public:
	StateManager(int stateTypeNum);
	virtual ~StateManager();

	/*
	* Init ȣ�� ������ AddArchiveData�� ȣ��Ǿ�� �մϴ�.
	*/
	virtual bool Init();
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
	
	virtual void Update(float deltaTime);
	
	FStateDesc GetCurStateDesc();

	/*
	* key : �����͸� ã���� ���� Ű���Դϴ�.
	* data : ������ �������Դϴ�. �������� �����ֱ�� �Լ��� ȣ���� ��ü�� å�������մϴ�.
	* return : ������ true, ���н� false�� �����մϴ�.
	*/
	bool AddArchiveData(FName key, void* data);
	void RemoveArchiveData(FName key);

	// UActorComponent
	

protected:

	void SetDefaultState(CharacterState state);
	void ChangeStateContainer(int index);
	std::shared_ptr<IState> AddStateData(int index, CharacterState stateName, std::shared_ptr<IState> newState);
	std::shared_ptr<IState> GetStateData(int index, CharacterState stateName);
	bool ChangeState(std::shared_ptr<IState> newState);
};
