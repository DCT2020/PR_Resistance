// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// PR_Resistance
#include "CState.h"
#include "PR_Resistance/Interface/IStaminaUser.h"
/**
 * 
 */
 class IStaminaProvider;
class PR_RESISTANCE_API UFire : public CState, public IStaminaUser
{
private:
	IStaminaProvider* mPProvider = nullptr;

public:
	UFire();
	~UFire();

	bool Begin(CharacterState prevState) override;
	void Update(float deltaTime) override;
	void End() override;

	void SetProvider(IStaminaProvider* provider) override;

protected:
	bool _Init() override;

};
