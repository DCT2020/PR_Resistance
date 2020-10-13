// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "PR_Resistance/StatesSystem/Status.h"
#include "PR_Resistance/Interface/IStaminaUser.h"
/**
 * 
 */
class PR_RESISTANCE_API URun : public CState , public IStaminaUser
{
protected:
	const float mSPUsePerSec = 0.05f;
	FStatus* mCharacterStatus = nullptr;
	float* mMaxWalkSpeed = nullptr;

	IStaminaProvider* mSPProvider = nullptr;
public:
	URun();
	~URun();

	virtual bool _Init() override;
	virtual bool Begin(CharacterState prevState) override;
	virtual void Update(float deltaTime) override;
	virtual void End() override;

	//interface
	void SetProvider(IStaminaProvider* provider) override;

};
