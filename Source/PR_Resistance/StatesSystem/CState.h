// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
};
