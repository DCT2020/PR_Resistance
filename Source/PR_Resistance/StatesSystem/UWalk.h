// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PR_Resistance/PR_Resistance.h"
#include "PR_Resistance/StatesSystem/CState.h"
#include "UWalk.generated.h"
/**
 * 
 */
 UCLASS()
class PR_RESISTANCE_API UWalk : public UCState
{
	GENERATED_BODY()
public:
	FStatus* mCharacterStatus = nullptr;
	float* mMaxWalkSpeed = nullptr;


	bool Begin(uint8 prevState) override;
	void Update(float deltaTime) override;
	void End() override;

public:
	UWalk();
	virtual ~UWalk();

protected:
	bool _Init() override;

 };
