// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/StateDesc.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "StateManager.generated.h"

/**
 * 
 */

 USTRUCT(BlueprintType)
 struct FChracterState
 {
	GENERATED_BODY()
	public:
	UPROPERTY()
		TMap<uint8, UCState*> mStateContainer;
 };

 UCLASS(ClassGroup = (FSM), meta = (BlueprintSpawnableComponent))
class PR_RESISTANCE_API UStateManager : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(Replicated)
		TArray<FChracterState> mStateContiners;

	UPROPERTY(Replicated)
		UCharacterDataArchive* mCDArchive = nullptr;

	UPROPERTY(Replicated)
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
	UFUNCTION(Reliable, Server)
		virtual void TryChangeState(uint8 stateType);
	/*
	* 현재 상태를 종료시킵니다.
	*/
	UFUNCTION(Reliable, Server)
	void SetStateEnd(uint8 stateType);
	/*
	* Priroty를 무시하고 즉시 상태를 변경합니다.
	*/
	UFUNCTION(Reliable, Server)
		void SetState(uint8 stateType);

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


	UFUNCTION(BlueprintCallable, Category = FSM, meta = (DisplayName = "AddStateData"))
		void AddStateData_bp(const int index, const uint8 stateName, const UCState* newState, UCState*& ret);

	UFUNCTION(BlueprintCallable, Category = FSM, meta = (DisplayName = "GetCurStateData"))
		void GetCurState_bp(UCState*& curState);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
public: //Event
	// 상태가 변할때마다 호출									prevState		curState
	DECLARE_EVENT_TwoParams(UStateManager, FChangeEvent, uint8, uint8);
	FChangeEvent& OnStateChange();

	inline void BindStateChangeCall(const TBaseDelegate<void, uint8, uint8> func)
	{
		mChangeEvent.Add(func);
	}

private: // Event
	FChangeEvent mChangeEvent;
protected:
        UFUNCTION(Reliable,Server)
	virtual void Update(float deltaTime);

	UCharacterDataArchive * GetCharacterDataArchive();
	void SetCharacterDataArchive(UCharacterDataArchive* archive);

	void ChangeStateContainer(int index);
	bool ChangeState(UCState* newState);
public:
#define AddStateData_uint8(Index, State, StateClass) AddStateData(Index, static_cast<uint8>(State),NewObject<StateClass>())
#define SetDefaultState_uint8(Index, State) SetDefaultState(Index, static_cast<uint8>(State))
	void SetDefaultState(int index, uint8 state);
	UCState* AddStateData(int index, uint8 stateName, UCState* newState, bool isDoInit = true);
	UCState* GetStateData(int index, uint8 stateName);
};
