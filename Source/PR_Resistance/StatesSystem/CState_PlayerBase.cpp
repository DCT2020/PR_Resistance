// Fill out your copyright notice in the Description page of Project Settings.


#include "CState_PlayerBase.h"

void UCState_PlayerBase::InitDatas(const FStateData_PlayerBase* const data)
{
	const FStateData_PlayerBase** temp = const_cast<const FStateData_PlayerBase**>(&mCurrentStateInfos);
	(*temp) = data;

	mbIsInited = true;
}

bool UCState_PlayerBase::_Init()
{

	if (!mbIsInited)
	{
		ABLOG(Fatal,TEXT("Please call UCState_PlayerBase::InitDatas(FStateData_PlayerBase) [%s]"), *this->GetClass()->GetName());
		return false;
	}

	return true;
}
