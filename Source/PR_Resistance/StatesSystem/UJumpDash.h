// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "PR_Resistance/Interface/IStaminaUser.h"
/**
 * 
 */
class PR_RESISTANCE_API UJumpDash : public CState , public IStaminaUser
{
protected:
	IStaminaProvider* mSPProvider = nullptr;
	FStatus* mCharacterStatus = nullptr;
	FVector* mVelocity = nullptr;
	FVector mDirection = FVector::ZeroVector;
	float* mMaxWalkSpeed = nullptr;
	float* mCharacterGScale = nullptr;

	float mOriginalGScale = 0.0f;
	float mElapsedTime = 0.0f;

public:
	UJumpDash();
	~UJumpDash();

	bool Begin(CharacterState prevState) override;
	void Update(float deltaTime) override;
	void End() override;
	void SetProvider(IStaminaProvider* provider) override;
protected:
	bool _Init() override;
};
