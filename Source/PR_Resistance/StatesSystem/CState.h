// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "PR_Resistance/StatesSystem/IState.h"
/**
 * 
 */

class CharacterDataArchive;
class PR_RESISTANCE_API CState :  public IState
{
private:
	CharacterDataArchive* mDataArchive = nullptr;
protected:
	FStateDesc mDesc;
public:
	CState();
	~CState();

	bool Init(CharacterDataArchive* archive) final override;
	FStateDesc GetStateDesc() override;


	virtual void SetStart() override;
	virtual void SetStop() override;

protected:
	virtual bool _Init() = 0;
	CharacterDataArchive * const GetCharacterDataArchive();

#define GetCharaDataWithLog(dataName, out)																										\
	{																																			\
		bool result = this->GetCharacterDataArchive()->GetData(dataName,out);																	\
		if(!result)																																\
		{																																		\
			UE_LOG(PRR,Error,TEXT("Character data can't found [FUNCTION] %s | [LINE] %s "),*FString(__FUNCTION__), *FString::FromInt(__LINE__));\
			GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Red,																				\
				FString::Printf(TEXT("Character data can't found [FUNCTION] %s | [LINE] %s "),*FString(__FUNCTION__), *FString::FromInt(__LINE__)));\
		}																																		\
	}
};
