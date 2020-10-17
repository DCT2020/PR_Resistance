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

	FChracterState mStates; // State����Ʈ�� �����Ϳ��� ������ �ֵ���
public:
	UStateManager();
	//UStateManager(int stateTypeNum = 1);
	//UStateManager(CharacterDataArchive * const archive);
	virtual ~UStateManager();

	/*
	* Init ȣ�� ������ AddArchiveData�� ȣ��Ǿ�� �մϴ�.
	*/
	virtual bool Init();
	virtual void LoadStates() {}
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
	
	// UActorComponent
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FStateDesc GetCurStateDesc();

	/*
	* key : �����͸� ã���� ���� Ű���Դϴ�.
	* data : ������ �������Դϴ�. �������� �����ֱ�� �Լ��� ȣ���� ��ü�� å�������մϴ�.
	* return : ������ true, ���н� false�� �����մϴ�.
	*/
	bool AddArchiveData(FName key, void* data);
	void RemoveArchiveData(FName key);

public: //Event
	// ���°� ���Ҷ����� ȣ��									prevState		curState
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
