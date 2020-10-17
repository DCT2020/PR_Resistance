// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/StateDesc.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "StateManager.generated.h"

/**
 * 
 */

 USTRUCT()
 struct FChracterState
 {
	GENERATED_BODY()
	public:
	UPROPERTY()
		TMap<CharacterState, UCState*> mStateContainer;
 };

 UCLASS()
class PR_RESISTANCE_API UStateManager : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY()
		TArray<FChracterState> mStateContiners;

	UPROPERTY()
		UCharacterDataArchive* mCDArchive = nullptr;

	UPROPERTY()
		UCState* mCurState = nullptr;

	TQueue<FStateDesc> mStateChangeCalls;

	FChracterState mStates; // State리스트는 에디터에서 받을수 있도록
public:
	UStateManager();
	//UStateManager(int stateTypeNum = 1);
	//UStateManager(CharacterDataArchive * const archive);
	virtual ~UStateManager();

	/*
	* Init 호출 이전에 AddArchiveData가 호출되어야 합니다.
	*/
	virtual bool Init();
	virtual void LoadStates() {}
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
	
	// UActorComponent
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FStateDesc GetCurStateDesc();

	/*
	* key : 데이터를 찾을때 쓰는 키값입니다.
	* data : 공유할 데이터입니다. 데이터의 생명주기는 함수를 호출한 객체가 책임져야합니다.
	* return : 성공시 true, 실패시 false를 리턴합니다.
	*/
	bool AddArchiveData(FName key, void* data);
	void RemoveArchiveData(FName key);

public: //Event
	// 상태가 변할때마다 호출									prevState		curState
	DECLARE_EVENT_TwoParams(UStateManager, FChangeEvent, CharacterState, CharacterState);
	FChangeEvent& OnStateChange();

	inline void BindStateChangeCall(const TBaseDelegate<void, CharacterState, CharacterState> func) 
	{
		mChangeEvent.Add(func);
	}

private: // Event
	FChangeEvent mChangeEvent;
protected:
	virtual void Update(float deltaTime);

	UCharacterDataArchive * GetCharacterDataArchive();
	void SetCharacterDataArchive(UCharacterDataArchive* archive);

	void SetDefaultState(int index, CharacterState state);
	void ChangeStateContainer(int index);
	UCState* AddStateData(int index, CharacterState stateName, UCState* newState);
	UCState* GetStateData(int index, CharacterState stateName);
	bool ChangeState(UCState* newState);
};
