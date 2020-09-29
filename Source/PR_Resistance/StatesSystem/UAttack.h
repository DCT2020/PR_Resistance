// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "PR_Resistance/Interface/IStaminaUser.h"
#include "PR_Resistance/Combo/Action.h"
/**
 * 
 */
 class ComboManager;
 class UAnimInstance;
 class UDataTable;
class PR_RESISTANCE_API UAttack : public CState, public IStaminaUser
{
protected:
	IStaminaProvider* mSPProvider = nullptr;
	FStatus* mCharacterStatus = nullptr;

	ComboManager* mComboManager = nullptr;
	ActionInput* AInput = nullptr;

public:
	UAttack();
	~UAttack();

	bool Begin(CharacterState prevState) override;
	void Update(float deltaTime) override;
	void End() override;
	void SetProvider(IStaminaProvider* provider) override;
	void SetStop() override;

protected:
	bool _Init() override;
	void StopAttack();
};
