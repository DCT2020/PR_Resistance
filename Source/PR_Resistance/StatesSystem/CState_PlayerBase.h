// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "CState_PlayerBase.generated.h"

/**
 * 
 */

 USTRUCT()
 struct FStateData_PlayerBase
 {
    GENERATED_BODY()

    CharacterState mCurMainState;
    CharacterState mCurSubState;
 };

UCLASS()
class PR_RESISTANCE_API UCState_PlayerBase : public UCState
{
	GENERATED_BODY()
	
public:
	void InitDatas(const FStateData_PlayerBase * const data);
protected:
    const FStateData_PlayerBase * const mCurrentStateInfos = nullptr;
    bool mbIsInited = false;

    virtual bool _Init() override;
};
