// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "UJump.generated.h"
/**
 * 
 */
 UCLASS()
class PR_RESISTANCE_API UJump : public UCState
{
	GENERATED_BODY()
public:
	UJump();
	~UJump();

	bool Begin(uint8 prevState) override;
	void Update(float deltaTime) override;
	void End() override;

protected:
	bool _Init() override;
};
