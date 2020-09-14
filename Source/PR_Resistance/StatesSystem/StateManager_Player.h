// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/StateManager.h"

// interface
#include "PR_Resistance/Interface/IStaminaProvider.h"
#include "PR_Resistance/Interface/IStaminaUser.h"
/**
 * 
 */
class PR_RESISTANCE_API StateManager_Player : 
	public StateManager, public IStaminaProvider, public IStaminaUser
{
protected:
	IStaminaProvider* mSPProvider = nullptr;
public:
	StateManager_Player();
	virtual ~StateManager_Player() override;


	bool Init() override;
	bool UseStamina(float usedStamina) override;
	void SetProvider(IStaminaProvider* provider) override;


};
