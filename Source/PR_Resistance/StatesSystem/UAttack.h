// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "PR_Resistance/Interface/IStaminaUser.h"
#include "PR_Resistance/Combo/Action.h"

#include "UAttack.generated.h"
/**
 * 
 */
 class ComboManager;
 class UAnimInstance;
 class UDataTable;
 UCLASS()
 class PR_RESISTANCE_API UAttack : public UCState, public IStaminaUser
{
	GENERATED_BODY()
protected:
	IStaminaProvider* mSPProvider = nullptr;
	FStatus* mCharacterStatus = nullptr;

	ComboManager* mComboManager = nullptr;
	ActionInput* AInput = nullptr;

public:
	UAttack();
	~UAttack();

	bool Begin(uint8 prevState) override;
	void Update(float deltaTime) override;
	void End() override;
	void SetProvider(IStaminaProvider* provider) override;
	void SetStop() override;

protected:
	bool _Init() override;
	void StopAttack();
};
