// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "PR_Resistance/Interface/IStaminaUser.h"
/**
 *	TODO: 공격불가상태 코딩해야함
 */
class PR_RESISTANCE_API UDodge : public CState, public IStaminaUser
{
protected:
	IStaminaProvider* mSPProvider = nullptr;
	FStatus* mChracterStatus = nullptr;

	//임시
	float* mMaxWalkSpeed = nullptr;
	float mElapsedTime = 0.0f;

	bool _Init() override;

public:
	UDodge();
	virtual ~UDodge();

	bool Begin(CharacterState prevState) override;
	void Update(float deltaTime) override;
	void End() override;

	void SetProvider(IStaminaProvider* provider) override;
};
