// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PR_Resistance/StatesSystem/CState.h"
/**
 * 
 */
class PR_RESISTANCE_API UJump : public CState
{
public:
	UJump();
	~UJump();

	bool Begin(CharacterState prevState) override;
	void Update(float deltaTime) override;
	void End() override;

protected:
	bool _Init() override;
};
