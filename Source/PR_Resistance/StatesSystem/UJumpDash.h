// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "PR_Resistance/Interface/IStaminaUser.h"
#include "UJumpDash.generated.h"
/**
 * 
 */
 UCLASS()
class PR_RESISTANCE_API UJumpDash : public UCState, public IStaminaUser
{
	GENERATED_BODY()
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

	bool Begin(uint8 prevState) override;
	void Update(float deltaTime) override;
	void End() override;
	void SetProvider(IStaminaProvider* provider) override;
protected:
	bool _Init() override;
};
