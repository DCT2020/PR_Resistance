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
	TMap<CharacterState,std::shared_ptr<IState>> mStates; // State리스트는 에디터에서 받을수 있도록
	std::shared_ptr<IState> mCurState = nullptr;
	CharacterDataArchive* mCDArchive = nullptr;
public:
	StateManager(int stateTypeNum);
	virtual ~StateManager();

	/*
	* Init 호출 이전에 AddArchiveData가 호출되어야 합니다.
	*/
	virtual bool Init();
	/*
	* State변경을 시도합니다.
	* FStateDesc의 Priority에 따라 실패하거나 성공합니다.
	* 다음 프레임에 시도됩니다.
	*/
	void TryChangeState(CharacterState stateType);
	/*
	* 현재 상태를 종료시킵니다.
	*/
	void SetStateEnd(CharacterState stateType);
	/*
	* Priroty를 무시하고 즉시 상태를 변경합니다.
	*/
	void SetState(CharacterState destateTypesc);
	
	virtual void Update(float deltaTime);
	
	FStateDesc GetCurStateDesc();

	/*
	* key : 데이터를 찾을때 쓰는 키값입니다.
	* data : 공유할 데이터입니다. 데이터의 생명주기는 함수를 호출한 객체가 책임져야합니다.
	* return : 성공시 true, 실패시 false를 리턴합니다.
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
