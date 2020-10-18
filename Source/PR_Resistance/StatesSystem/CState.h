// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "UObject/NoExportTypes.h"

// My Class
#include "PR_Resistance/StatesSystem/Status.h"
#include "PR_Resistance/StatesSystem/StateDesc.h"
#include "PR_Resistance/StatesSystem/CharacterDataArchive.h"

#include "CState.generated.h"

/**
 * 
 */
UCLASS()
class PR_RESISTANCE_API UCState : public UObject
{
	GENERATED_BODY()
private:
	UCharacterDataArchive* mDataArchive = nullptr;
protected:
	FStateDesc mDesc;
public:
	UCState();
	~UCState();

	bool Init(UCharacterDataArchive* archive);
	FStateDesc GetStateDesc();


	virtual void SetStart();
	virtual void SetStop();

	virtual bool Begin(uint8 prevState);
	virtual void Update(float deltaTime);
	virtual void End();

protected:
	virtual bool _Init();
	UCharacterDataArchive * GetCharacterDataArchive();

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